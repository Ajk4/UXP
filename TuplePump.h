#ifndef TUPLE_PUMP_H
#define TUPLE_PUMP_H

#include <string>

class Tuple;
class TupleSystem;
class TupleMatcher;

/*
 * klasa transportujaca krotki pomiedzy procesami
 */
class TuplePump {
private:
	friend class TupleSystem;

	static std::string tupleFIFO;
	static std::string infoFIFO;

	std::string tupleName; //nazwa wewntrznego FIFO do synchronizacji odbierania krotek
	std::string infoName; //nazwa wewntrznego FIFO do synchronizacji odbierania informacji
	std::string name; //nazwa FIFO do komunikacji miedzyprocesowej
	TupleMatcher *matcher;
	bool running; //flaga czy pompa ma pompowac czy sie odlaczyc
	bool correctlyInitialized; //czy poprawnie udalo sie zainicjalizowac pompe (wszystkie deskryptory etc)

	int fifo[2]; //deskryptory FIFO do komunikacji miedzyprocesowej
	int tupleFD[2]; //deskryptory FIFO/pipe sluzacy do odbierania krotek od API (wewnetrzny, wysylajcy fd jest w putTuple, a odbierajacy wewnatrz start)
	int infoFD[2]; //deskryptory FIFO/pipe sluzacy do odbioru ktorek o informacji o zakonczeniu itp (wewnetrzne wysylajcy fd jest w stop, a odbierajacy wewnatrz start)

	/**
	 * Tylko TupleSystem moze utworzyc pompe i moze ja zniszczyc
	 * name - nazwa dla potoku nazwanego
	 * matcher - przypisany matcher, do ktorego beda wysylane pobrane krotki z potoku
	 */
	TuplePump(const char *name, TupleMatcher *matcher);

	~TuplePump(void);

	/**
	 * Dolacza do potoku nazwanego i wykonuje obsluge zwiazana z pobieraniem krotek,
	 * Wysylaniem ich do matchera i odsylaniem z powrotem do przestrzeni.
	 * Wysyla takze nowe krotki w przestrzen.
	 *
	 * Metoda dzialajaca w petli, dopoki nie zostanie wywolana metoda stop.
	 *
	 * Zwraca -1 jezeli nie udalo sie dolaczyc do przestrzeni
	 */
	int start(void);

	/**
	 * Zakancza dzialanie pompy i odlacza ja od potoku nazwanego
	 */
	void stop(void);

	/**
	 * Umieszcza krotke w przestrzeni.
	 */
	void putTuple(const Tuple *t);
};

#endif
