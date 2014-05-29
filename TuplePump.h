#ifndef TUPLE_PUMP_H
#define TUPLE_PUMP_H

#include <string>
#include <pthread.h>

class Tuple;
class TupleSystem;
class TupleMatcher;

/*
 * klasa transportujaca krotki pomiedzy procesami
 */
class TuplePump {
private:

	static std::string tupleFIFO;
	static std::string infoFIFO;

	std::string tupleName; //nazwa wewntrznego FIFO do synchronizacji odbierania krotek
	std::string infoName; //nazwa wewntrznego FIFO do synchronizacji odbierania informacji o zakonczeniu
	std::string name; //nazwa FIFO do komunikacji miedzyprocesowej

	TupleMatcher *matcher;
	bool running; //flaga czy pompa ma pompowac czy sie odlaczyc
	bool correctlyInitialized; //czy poprawnie udalo sie zainicjalizowac pompe (wszystkie deskryptory etc)
	pthread_t thread;

	int fifoFD; //deskryptor FIFO do komunikacji miedzyprocesowej
	int tupleFD; //deskryptor FIFO sluzacy do odbierania krotek od API (wewnetrzny, wysylajcy fd jest w putTuple, a odbierajacy wewnatrz start)
	int infoFD; //deskryptor FIFO sluzacy do odbioru ktorek o informacji o zakonczeniu itp (wewnetrzne wysylajcy fd jest w stop, a odbierajacy wewnatrz start)

	//informacje jakie przez infoFD beda przesylane
	enum information{
		TERMINATE
	};

	/*
	 * wykonuje obsluge zwiazana z pobieraniem krotek,
	 * Wysylaniem ich do matchera i odsylaniem z powrotem do przestrzeni.
	 * Wysyla takze nowe krotki w przestrzen.
	 *
	 * Metoda dzialajaca w petli, dopoki nie zostanie wywolana metoda stop.
	 */
	static void *_run(void *ptr);

public:
	/**
	 * name - nazwa dla potoku nazwanego
	 * matcher - przypisany matcher, do ktorego beda wysylane pobrane krotki z potoku
	 */
	TuplePump(const char *name, TupleMatcher *matcher);

	~TuplePump(void);

	/**
	 * Uruchamia pompe poprzez otworzenie potokow wewnetrzenych
	 * i dolaczeniu do potoku nazwanego do komunikacji miedzyprocesowej.
	 *
	 * Uruchamia obsluge krotek, ktora bedzie sie dziac w oddzielnym watku.
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
