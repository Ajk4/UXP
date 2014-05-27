#include "TuplePump.h"
#include "TupleMatcher.h"
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <cstdio>

#define FIFO_READ 0
#define FIFO_WRITE 1
#define PERMS 0666

std::string TuplePump::tupleFIFO = "/tmp/tupleFIFO.";
std::string TuplePump::infoFIFO = "/tmp/infoFIFO.";

TuplePump::TuplePump(const char *name, TupleMatcher *matcher) :
		name(name), matcher(matcher), running(false), correctlyInitialied(true), fifo {
				-1, -1 }, tupleFD { -1, -1 }, infoFD { -1, -1 } {

	//int mknod(const char *pathname, mode_t mode, dev_t dev);

	if (mknod((tupleFIFO + std::to_string(getpid())).c_str(), S_IFIFO | PERMS,
			0) < 0) {
		std::fprintf(stderr, "nie mozna utworzyc FIFO wewnetrznego: %s",
				(tupleFIFO + std::to_string(getpid())).c_str());
		correctlyInitialied = false;
	}

	if (mknod((infoFIFO + std::to_string(getpid())).c_str(), S_IFIFO | PERMS, 0)
			< 0) {
		std::fprintf(stderr, "nie mozna utworzyc FIFO wewnetrznego: %s",
				(infoFIFO + std::to_string(getpid())).c_str());
		correctlyInitialied = false;
	}

	if ((mknod(this->name.c_str(), S_IFIFO | PERMS, 0) < 0) && (errno != EEXIST)) {
		std::fprintf(stderr, "nie mozna utworzyc FIFO miedzyprocesowego: %s",
				this->name.c_str());
		correctlyInitialied = false;
	}

}

TuplePump::~TuplePump(void) {
	stop();

}

int TuplePump::start(void) {
	//otworz potoki, stworz nowy watek,w masce ustaw brak przechwytywania jakichkolwiek sygnalow
	return -1;
}

void TuplePump::stop(void) {
	close(fifo[0]);
	close(fifo[1]);
	close(tupleFD[0]);
	close(tupleFD[1]);
	close(infoFD[0]);
	close(infoFD[1]);

	if (unlink( (tupleFIFO + std::to_string(getpid())).c_str()) < 0) {
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
