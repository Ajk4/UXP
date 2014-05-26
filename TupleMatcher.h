#ifndef TUPLE_MATCHER_H
#define TUPLE_MATCHER_H

class TuplePattern;
/*
 * klasa sprawdzajaca dopasowanie krotki do wzorca
 */
class TupleMatcher {
	//wzorzec, ktorego dopasowania szuka
	TuplePattern *pattern;

	//deskryptory FIFO/pipe do komunikacji z API
	int tupleSendFD; //odsylanie krotki
	int infoFD[2]; //odbior informacji od API (np. o timeoucie) (wewnetrzne, w timeoutOccured
	//jest deskryptor "wysylajacy", a w match "odbierajacy")

	//wewnetrzny deskryptor FIFO/pipe do odbioru wzorcow krotek (wewnetrzne, w putPattern
	//jest deskryptor "wysylajacy", a w match "odbierajacy")
	int patternFD [2];

public:
	TupleMatcher(int tupleSendFD);

	/*
	 * Otrzymuje krotke, ktora nastepnie porownuje ze wzorcem jezeli istnieje
	 * (wczesniej wykonuje czasowe zatrzymanie sie na odbiorze wzorca jezeli go nie ma)
	 * a nastepnie probuje dopasowac wzorzec do otrzymanej w parametrze krotki.
	 * Sprawdzany jest tez deskryptor informacji, czy nie zaszedl timeout w API co powoduje
	 * usuniecie oczekiwania na okreslony wzorzec.
	 * Jezeli dopasowanie jest udane zwraca krotke poprzez deskryptor tupleSendFD i usuwa
	 * informacje o tym ze oczekuje na na jakis wzorzec
	 *
	 * binaryTuple - krotka w binarnej postaci
	 *
	 * Wartosc zwracana:
	 * -1 - brak dopasowania
	 * 0 - dopasowanie udane, operacja input
	 * 1 - dopasowanie udane, operacja read
	 */
	int match(unsigned char *binaryTuple);

	//Ustawia wzorzec do szukania na ten z parametru
	void putPattern(TuplePattern *pattern);

	//informuje o zajsciu timeoutu
	void timeoutOccured(void);
};

#endif
