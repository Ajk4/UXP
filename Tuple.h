#ifndef TUPLE_H
#define TUPLE_H

#include <limits>
#include <string>
#include "OperationCode.h"

#define TUPLE_ELEMENTS 		10
#define MAX_STRING_LEN		10 //ze znakiem \0!

//wartosci danych a krotki interpretowane jako niepoprawne
#define INVALID_STRING 		""
#define INVALID_INT 		std::numeric_limits<int>::max()
#define INVALID_FLOAT 		std::numeric_limits<float>::max()

#define BINARY_TUPLE_LENGTH sizeof(int) + (TUPLE_ELEMENTS * (MAX_STRING_LEN + 1))
#define DEFAULT_TTL			std::numeric_limits<short int>::max()

/*
 * klasa reprezentująca dane krotki
 */
class Tuple {
public:
	struct TupleElement;
private:

	//przechowuje elementy krotki
	TupleElement *elements[TUPLE_ELEMENTS];
    int actualTupleIndex = 0;

public:
	Tuple(void);
	~Tuple(void);

	//dopisuje do krotki nowy element, wartosc zwracana informuje o wyniku operacji
    int append(const std::string &value);
    int append(int value);
    int append(float value);

	//zwracaja w drugim parametrze wartosc z podanego indeksu
	//w krotce
    int get(int idx, std::string &out);
    int get(int idx, int &out);
    int get(int idx, float &out);

	//konwertuje krotke na jej postac binarna i zapisuje do buf
	void getBinaryRepresentation(unsigned char *buf, int ttl);

	//konwertuje postac binarna do krotki
	void convertBinaryToTuple(unsigned char *buf, Tuple &t);
};

/**
 * Przechowuje informacje o jednym elemencie krotki
 */
struct Tuple::TupleElement {
	//typ danych przechowywany w strukturze
	enum Type {
		UNKNOWN = 0,
		STRING,
		INT,
		FLOAT,

	};

	//przechowywany typ danych
	int dataType;

	//dane
	std::string str;
	int i;
	float f;

	//zwracaja dana, powinny informowac o bledzie
	//jezeli nei ten typ danych jest przechowywany
	//w danej instancji struktury
	void get(std::string &out);
	void get(int &out);
	void get(float &out);

	//konstruuje obiekt w zaleznosci od podanego typu danych
	TupleElement(const std::string &s);
	TupleElement(int i);
	TupleElement(float f);
};

#endif
