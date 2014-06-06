#ifndef TUPLE_PATTERN_H
#define TUPLE_PATTERN_H

#include <string>
#include "Tuple.h"
#include "OperationCode.h"

/*
 * klasa reprezentująca wzorzec krotki
 */
class TuplePattern {
	friend class TupleSystem;
	friend class TupleMatcher;

	enum opType {
			INPUT = 0,
			READ
		};

	//typ operacji jaki jest niesiony z wzorcem
	int operation;

    int actualElementIndex;

	//ustawia typ operacji dla wzorca
	void setOperationType(int op);
public:
	enum relOp {
		GT = 0, //>
		LT,		//<
		GE,		//>=
		LE,		//<=
		EQ,		//==
		ANY		//*
	};

	struct TuplePatternElement;

	//przechowuje elementy wzorca
	TuplePatternElement *elements[TUPLE_ELEMENTS];



    int appendString(relOp op, std::string value = INVALID_STRING);
    int appendInt(relOp op, int value = INVALID_INT);
    int appendFloat(relOp op, float value = INVALID_FLOAT);

    TuplePattern(void);
    ~TuplePattern(void);
};

/**
 * klasa reprezentujaca jeden element wzorca, zawiera to co zwykly element krotki
 * i dodatkowo operacje relacyjna
 */
struct TuplePattern::TuplePatternElement : public Tuple::TupleElement{
	//przechowywany operator relacji
	int relOP;

	//konstruuje obiekt w zaleznosci od podanego typu danych
    TuplePatternElement(int relOp, const std::string &s);
    TuplePatternElement(int relOp, int i);
    TuplePatternElement(int relOp, float f);
};

#endif
