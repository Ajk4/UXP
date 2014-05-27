#include "TuplePump.h"
#include "TupleMatcher.h"
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <string>
#include <cstdio>

#define FIFO_READ 0
#define FIFO_WRITE 1
#define PERMS 0666

std::string TuplePump::tupleFIFO = "/tmp/tupleFIFO.";
std::string TuplePump::infoFIFO = "/tmp/infoFIFO.";

TuplePump::TuplePump(const char *name, TupleMatcher *matcher) :
		name(name), matcher(matcher), running(false), correctlyInitialized(true), fifo {
				-1, -1 }, tupleFD { -1, -1 }, infoFD { -1, -1 } {

	//int mknod(const char *pathname, mode_t mode, dev_t dev);

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

	if ((fifo[FIFO_WRITE] = open(name.c_str(), O_RDWR) < 0)) {
		std::fprintf(stderr, "nie mozna otworzyc FIFO wewnetrznego: %s",
				infoName.c_str());
		start = false;
	}

	if ((fifo[FIFO_READ] = open(name.c_str(), O_RDWR) < 0)) {
		std::fprintf(stderr, "nie mozna otworzyc FIFO wewnetrznego: %s",
				name.c_str());
		start = false;
	}


	if(start) {

		std::printf("wszystko ok\n");
	}

	return -1;
}

void TuplePump::stop(void) {
	close(fifo[0]);
	close(fifo[1]);
	close(tupleFD[0]);
	close(tupleFD[1]);
	close(infoFD[0]);
	close(infoFD[1]);

	if (unlink((tupleFIFO + std::to_string(getpid())).c_str()) < 0) {
		std::fprintf(stderr, "nie mozna odlaczyc FIFO wewnetrznego: %s",
				(tupleFIFO + std::to_string(getpid())).c_str());
	}

	if (unlink((infoFIFO + std::to_string(getpid())).c_str()) < 0) {
		std::fprintf(stderr, "nie mozna odlaczyc FIFO wewnetrznego: %s",
				(infoFIFO + std::to_string(getpid())).c_str());
	}

	if (unlink(name.c_str()) < 0) {
		std::fprintf(stderr, "nie mozna odlaczyc FIFO miedzyprocesowego: %s",
				name.c_str());
	}
}

void TuplePump::putTuple(const Tuple *t) {

}
