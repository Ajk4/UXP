#include "Tuple.h"
#include "TuplePump.h"
#include "TupleMatcher.h"
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <cstdio>
#include <cstring>

#define PERMS 0666
#define LOOP_SLEEP_TIME_USEC 50000

std::string TuplePump::tupleFIFO = "/tmp/tupleFIFO.";
std::string TuplePump::infoFIFO = "/tmp/infoFIFO.";

TuplePump::TuplePump(const char *name, TupleMatcher *matcher) :
		name(name), matcher(matcher), running(false), correctlyInitialized(
				true), fifoFD(-1), tupleFD(-1), infoFD(-1) {

	tupleName = tupleFIFO + std::to_string(getpid());
	infoName = infoFIFO + std::to_string(getpid());

	if (mknod(tupleName.c_str(), S_IFIFO | PERMS, 0) < 0) {
		std::fprintf(stderr, "nie mozna utworzyc FIFO wewnetrznego: %s",
				tupleName.c_str());
		correctlyInitialized = false;
	}

	if (mknod(infoName.c_str(), S_IFIFO | PERMS, 0) < 0) {
		std::fprintf(stderr, "nie mozna utworzyc FIFO wewnetrznego: %s",
				infoName.c_str());
		correctlyInitialized = false;
	}

	if ((mknod(this->name.c_str(), S_IFIFO | PERMS, 0) < 0)
			&& (errno != EEXIST)) {
		std::fprintf(stderr, "nie mozna utworzyc FIFO miedzyprocesowego: %s",
				this->name.c_str());
		correctlyInitialized = false;
	}

}

TuplePump::~TuplePump(void) {
	stop();

}

int TuplePump::start(void) {
	//otworz potoki, stworz nowy watek,w masce ustaw brak przechwytywania jakichkolwiek sygnalow

	if (!correctlyInitialized) {
		stop();
		return -1;
	}

	//zmienna czy mozna zastartowac po dolaczeniu do potokow
	bool start = true;

	if ((tupleFD = open(tupleName.c_str(), O_RDWR)) < 0) {
		std::fprintf(stderr, "nie mozna otworzyc FIFO wewnetrznego: %s\n",
				tupleName.c_str());
		start = false;
	}

	if ((infoFD = open(infoName.c_str(), O_RDWR)) < 0) {
		std::fprintf(stderr, "nie mozna otworzyc FIFO wewnetrznego: %s\n",
				infoName.c_str());
		start = false;
	}

	if ((fifoFD = open(name.c_str(), O_RDWR)) < 0) {
		std::fprintf(stderr, "nie mozna otworzyc FIFO wewnetrznego: %s\n",
				name.c_str());
		start = false;
	}

	if(start) {
		running = true;
		if ((pthread_create(&thread, nullptr, TuplePump::_run, (void*) this))
				!= 0) {
			std::fprintf(stderr, "Nie mozna utworzyc nowego watku\n");
			start = false;
			running = false;
		}
	}

	if (!start) {
		std::fprintf(stderr, "Wystapily bledy, nie mozna wystartowac\n");
		return -1;
	}

	return 0;
}

void *TuplePump::_run(void *ptr) {

	auto pump = (TuplePump*) ptr;

	//znajdz deskryptor o najwieszej wartosci dla funkcji select
	int maxFD =
			pump->fifoFD > pump->infoFD ?
					(pump->fifoFD > pump->tupleFD ?
							pump->fifoFD : pump->tupleFD) :
					(pump->infoFD > pump->tupleFD ?
							pump->infoFD : pump->tupleFD);

	int sel;
	unsigned char buf[BINARY_TUPLE_LENGTH];
	fd_set readSet;

	do {

		FD_ZERO(&readSet);

		FD_SET(pump->fifoFD, &readSet);
		FD_SET(pump->infoFD, &readSet);
		FD_SET(pump->tupleFD, &readSet);

		struct timeval tv;
		tv.tv_sec = 1;
		tv.tv_usec = 0;

		//zaczekaj na dostepnosc deskryptora
		sel = select(maxFD + 1, &readSet, NULL, NULL, &tv);

		if (sel < 0) {
			std::fprintf(stderr, "select error\n");
			return ptr;
		} else if (sel == 0) {
			std::fprintf(stderr, "timeout\n");
		} else {
			if (FD_ISSET(pump->tupleFD, &readSet)) {
				//odbierz krotke do wyslania w przestrzen


				read(pump->tupleFD, buf, BINARY_TUPLE_LENGTH);

				std::fprintf(stderr, "odebrano krotke do wysylki: %c\n", buf[4]);

				int i = write(pump->fifoFD, buf, BINARY_TUPLE_LENGTH);
				std::fprintf(stderr, "wysylanie, kod wyslania: %d/%d\n", i, BINARY_TUPLE_LENGTH);
				//write(pump->fifo[FIFO_WRITE], buf, BINARY_TUPLE_LENGTH);
			} else if (FD_ISSET(pump->infoFD, &readSet)) {
				//dotarla jakas informacja (np o zakocnzeniu)
				//informacje obsluguje sie kiedy nie ma nic do wysylki
				//dzieki czemu unika sie sytuacji kiedy po kilku wrzuceniach krotek
				//puszcza sie informacje o stopie wszystkie krotki zostana wyslane
				//zanim ten stop zajdzie
				int data[1];
				read(pump->infoFD, data, 1);

				if(data[0] == TERMINATE) {
					std::fprintf(stderr, "konczenie\n");
					return ptr;
				}

			}

			if (FD_ISSET(pump->fifoFD, &readSet)) {
				//standardowe przepompowywanie krotek
				read(pump->fifoFD, buf, BINARY_TUPLE_LENGTH);

				int ttl;
				std::memcpy(&ttl, buf, 4);
				int pid;
				std::memcpy(&pid, buf + 5, 4);
				std::fprintf(stderr, "[%d] odebrano krotke z przestrzeni: %c, ttl = %d, senderPid = %d\n", getpid(), buf[4], ttl, pid);

				/*int result = pump->matcher->match(buf);
				if(result == -1 || result == 1) {*/
				//zmniejszyc TTL i wywalic krotke do przestrzeni z powrotem
					--ttl;
					if(ttl > 0) {
						std::memcpy(buf, &ttl, 4);
						write(pump->fifoFD, buf, BINARY_TUPLE_LENGTH);
					}
				/*}
				*/

			}

			//loop sleep
			usleep(LOOP_SLEEP_TIME_USEC);
		}
	} while (pump->running);

	return ptr;
}

void TuplePump::stop(void) {

	//wyslij informacje o zakonczeniu
	int info[] = { 1 };
	write(infoFD, info, sizeof(info));

	//poczekaj na watek obslugujacy pompe
	pthread_join(thread, nullptr);

	//pozamykaj wszystkie deskryptory
	close(fifoFD);
	close(tupleFD);
	close(infoFD);

	unlink((tupleFIFO + std::to_string(getpid())).c_str());

	unlink((infoFIFO + std::to_string(getpid())).c_str());

	unlink(name.c_str());
}

void TuplePump::putTuple(const Tuple *t) {
	static unsigned char c = (unsigned char)0;//[PH]dane "krotki"
	unsigned char buf[BINARY_TUPLE_LENGTH];
	std::memset(buf, 0, BINARY_TUPLE_LENGTH);//[PH]
	//t->getBinaryRepresentation(buf, DEFAULT_TTL); //NYI
	int ttl = 10;
	std::memcpy(buf, &ttl, 4);
	int pidSender = getpid();
	std::memcpy(buf + 5, &pidSender, 4);
	buf[4] = ((++c) % 90) + 33;//[PH] dane binarne krotki

	write(tupleFD, buf, BINARY_TUPLE_LENGTH);

}
