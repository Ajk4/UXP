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

#define FIFO_READ 0
#define FIFO_WRITE 1
#define PERMS 0666

std::string TuplePump::tupleFIFO = "/tmp/tupleFIFO.";
std::string TuplePump::infoFIFO = "/tmp/infoFIFO.";

TuplePump::TuplePump(const char *name, TupleMatcher *matcher) :
		name(name), matcher(matcher), running(false), correctlyInitialized(
				true), fifoFD { -1, -1 }, tupleFD { -1, -1 }, infoFD { -1, -1 } {

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

	if ((tupleFD[FIFO_WRITE] = open(tupleName.c_str(), O_RDWR)) < 0) {
		std::fprintf(stderr, "nie mozna otworzyc FIFO wewnetrznego: %s",
				tupleName.c_str());
		start = false;
	}

	if ((tupleFD[FIFO_READ] = open(tupleName.c_str(), O_RDWR)) < 0) {
		std::fprintf(stderr, "nie mozna otworzyc FIFO wewnetrznego: %s",
				tupleName.c_str());
		start = false;
	}

	if ((infoFD[FIFO_WRITE] = open(infoName.c_str(), O_RDWR)) < 0) {
		std::fprintf(stderr, "nie mozna otworzyc FIFO wewnetrznego: %s",
				infoName.c_str());
		start = false;
	}

	if ((infoFD[FIFO_READ] = open(infoName.c_str(), O_RDWR)) < 0) {
		std::fprintf(stderr, "nie mozna otworzyc FIFO wewnetrznego: %s",
				infoName.c_str());
		start = false;
	}

	if ((fifoFD[FIFO_WRITE] = open(name.c_str(), O_RDWR)) < 0) {
		std::fprintf(stderr, "nie mozna otworzyc FIFO wewnetrznego: %s",
				infoName.c_str());
		start = false;
	}

	if ((fifoFD[FIFO_READ] = open(name.c_str(), O_RDWR)) < 0) {
		std::fprintf(stderr, "nie mozna otworzyc FIFO wewnetrznego: %s",
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
	std::fprintf(stderr, "new thread\n");

	//znajdz deskryptor o najwieszej wartosci
	int maxFD =
			pump->fifoFD[FIFO_READ] > pump->infoFD[FIFO_READ] ?
					(pump->fifoFD[FIFO_READ] > pump->tupleFD[FIFO_READ] ?
							pump->fifoFD[FIFO_READ] : pump->tupleFD[FIFO_READ]) :
					(pump->infoFD[FIFO_READ] > pump->tupleFD[FIFO_READ] ?
							pump->infoFD[FIFO_READ] : pump->tupleFD[FIFO_READ]);


	std::fprintf(stderr, "FDs: %d, %d, %d, maxFD = %d\n", pump->fifoFD[FIFO_READ], pump->infoFD[FIFO_READ], pump->tupleFD[FIFO_READ], maxFD);
	int sel;
	unsigned char buf[BINARY_TUPLE_LENGTH];

	do {
		fd_set readSet;
		FD_ZERO(&readSet);

		FD_SET(pump->fifoFD[FIFO_READ], &readSet);
		FD_SET(pump->infoFD[FIFO_READ], &readSet);
		FD_SET(pump->tupleFD[FIFO_READ], &readSet);

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
			if (FD_ISSET(pump->tupleFD[FIFO_READ], &readSet)) {
				//odbierz krotke do wyslania w przestrzen


				read(pump->tupleFD[FIFO_READ], buf, BINARY_TUPLE_LENGTH);

				std::fprintf(stderr, "odebrano krotke do wysylki: %c\n", buf[4]);

				int i = write(pump->fifoFD[FIFO_WRITE], buf, BINARY_TUPLE_LENGTH);
				std::fprintf(stderr, "wysylanie, kod wyslania: %d/%d\n", i, BINARY_TUPLE_LENGTH);
				//write(pump->fifo[FIFO_WRITE], buf, BINARY_TUPLE_LENGTH);
			} else if (FD_ISSET(pump->infoFD[FIFO_READ], &readSet)) {
				//dotarla jakas informacja (np o zakocnzeniu)
				//informacje obsluguje sie kiedy nie ma nic do wysylki
				//dzieki czemu unika sie sytuacji kiedy po kilku wrzuceniach krotek
				//puszcza sie informacje o stopie wszystkie krotki zostana wyslane
				//zanim ten stop zajdzie
				int data[1];
				read(pump->infoFD[FIFO_READ], data, 1);

				if(data[0] == TERMINATE) {
					std::fprintf(stderr, "konczenie\n");
					return ptr;
				}

			}

			if (FD_ISSET(pump->fifoFD[FIFO_READ], &readSet)) {
				//standardowe przepompowywanie krotek
				read(pump->fifoFD[FIFO_READ], buf, BINARY_TUPLE_LENGTH);

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
						write(pump->fifoFD[FIFO_WRITE], buf, BINARY_TUPLE_LENGTH);
					}
				/*}
				*/

			}
			//50ms sleep
			usleep(50000);
		}
	} while (pump->running);

	return ptr;
}

void TuplePump::stop(void) {

	//wyslij informacje o zakonczeniu
	int info[] = { 1 };
	write(infoFD[FIFO_WRITE], info, sizeof(info));

	//poczekaj na watek obslugujacy pompe
	pthread_join(thread, nullptr);

	//pozamykaj wszystkie deskryptory
	close(fifoFD[0]);
	close(fifoFD[1]);
	close(tupleFD[0]);
	close(tupleFD[1]);
	close(infoFD[0]);
	close(infoFD[1]);

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

	write(tupleFD[FIFO_WRITE], buf, BINARY_TUPLE_LENGTH);

}
