#ifndef TUPLE_SYSTEM_H
#define TUPLE_SYSTEM_H

#include "TuplePump.h"
#include "TupleMatcher.h"

class Tuple;
class TupleMatcher;
class TuplePattern;
class TuplePump;

/*
 * klasa zapewniajaca API do wysyłania/odbierania krotek,oraz dolaczania/odlaczania do i z przestrzeni krotek
 */
class TupleSystem {
private:
	//poprzez FIFO/pipe pokazywane przez ten deskryptor przyjdzie krotka
	int tupleRecvFD;

	TupleMatcher *matcher;
	TuplePump *pump;
public:
	~TupleSystem(void);

	//dolacza do przestrzeni krotek o nazwie name
	void lindaOpen(const char *name);

	//odlacza sie od przestrzeni krotek
	void lindaClose(void);

	//umieszcza krotke w przestrzeni
	void lindaOutput(Tuple *tuple);

	/*
	 * Zawiesza sie w oczekiwaniu na krotke o zadanym wzorcu, czekajac
	 * maksymalnie timout (sekund/milisekund?). Po znalezieniu pasujacej
	 * krotki wypelnia podany wskaznik danymi i zwraca 0.
	 *
	 * Jezeli operacja nie powiodla sie i doszlo do timeoutu zwraca -1
	 * i stan output jest nieokreslony
	 *
	 * Operacja Input usuwa krotke z przestrzeni, Read pozostawia ja.
	 */
	int lindaInput(TuplePattern *pattern, int timeout, Tuple *output);
	int lindaRead(TuplePattern *pattern, int timeout, Tuple *output);
};

#endif
