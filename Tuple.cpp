#include "Tuple.h"
#include <cstring>
Tuple::Tuple()
{
	actualTupleIndex = 0;
	for(int i = 0; i < TUPLE_ELEMENTS ; ++i) {
		elements[i] = NULL;
	}
}
Tuple::~Tuple()
{
	for(int i = 0; i < actualTupleIndex ; ++i) {
		delete elements[i];
	}
}
int Tuple::append(const std::string &value)
{
    if(actualTupleIndex >= TUPLE_ELEMENTS){
        return OperationCode::TUPLE_FULL;
    } else {
        elements[actualTupleIndex] = new TupleElement(value);
        actualTupleIndex++;
        return OperationCode::OK;
    }
}

int Tuple::append(int value)
{
    if(actualTupleIndex >= TUPLE_ELEMENTS){
        return OperationCode::TUPLE_FULL;
    } else {
        elements[actualTupleIndex] = new TupleElement(value);
        actualTupleIndex++;
        return OperationCode::OK;
    }
}

int Tuple::append(float value)
{
    if(actualTupleIndex >= TUPLE_ELEMENTS){
        return OperationCode::TUPLE_FULL;
    } else {
        elements[actualTupleIndex] = new TupleElement(value);
        actualTupleIndex++;
        return OperationCode::OK;
    }
}

int Tuple::get(int idx, std::string &out)
{
    if(idx < 0 || idx > actualTupleIndex){
        return OperationCode::INDEX_OUT_OF_BOUNDS;
    } else {
        auto tupleElement = elements[idx];
        if(tupleElement->dataType != Tuple::TupleElement::STRING){
            return OperationCode::WRONG_TYPE;
        }
        out = tupleElement->str;
        return OperationCode::OK;
    }
}

int Tuple::get(int idx, int &out)
{
    if(idx < 0 || idx > actualTupleIndex){
        return OperationCode::INDEX_OUT_OF_BOUNDS;
    } else {
        auto tupleElement = elements[idx];
        if(tupleElement->dataType != Tuple::TupleElement::INT){
            return OperationCode::WRONG_TYPE;
        }
        out = tupleElement->i;
        return OperationCode::OK;
    }
}

int Tuple::get(int idx, float &out)
{
    if(idx < 0 || idx > actualTupleIndex){
        return OperationCode::INDEX_OUT_OF_BOUNDS;
    } else {
        auto tupleElement = elements[idx];
        if(tupleElement->dataType != Tuple::TupleElement::FLOAT){
            return OperationCode::WRONG_TYPE;
        }
        out = tupleElement->f;
        return OperationCode::OK;
    }
}

void Tuple::getBinaryRepresentation(unsigned char *buf, int ttl) {

	//wyresetuj bufor
	std::memset(buf, 0, BINARY_TUPLE_LENGTH);

	//zapisz ttl
	std::memcpy(buf, &ttl, sizeof(ttl));

	auto curBufPos = buf + sizeof(int);
	float tupleF = 0.0f;
	int tupleI = 0;
	std::string tupleS = "";

	for(int i = 0; i < actualTupleIndex; ++i) {
		//zapisz typ elementu
		*curBufPos = (unsigned char)elements[i]->dataType;
		//przesun wskaznik na dane
		++curBufPos;

		//zapisz dane
		switch(elements[i]->dataType) {
		case TupleElement::FLOAT:
			tupleF = elements[i]->f;
			std::memcpy(curBufPos, &tupleF, sizeof(tupleF));
			break;
		case TupleElement::INT:
			tupleI = elements[i]->i;
			std::memcpy(curBufPos, &tupleI, sizeof(tupleI));
		case TupleElement::STRING:
			tupleS = elements[i]->str;
			std::memcpy(curBufPos, tupleS.c_str(), tupleS.size());
		}

		curBufPos += MAX_STRING_LEN;
	}

	//dane sie skonczyly, powpisuj nieznany typ danych
	for(int i = actualTupleIndex; i < MAX_STRING_LEN; ++i) {
		*curBufPos = TupleElement::UNKNOWN;
		curBufPos += MAX_STRING_LEN + 1;
	}
}


Tuple::TupleElement::TupleElement(const std::string &strVal)
{
    dataType = STRING;
    str = strVal;
}

Tuple::TupleElement::TupleElement(int intVal)
{
    dataType = INT;
    i = intVal;
}

Tuple::TupleElement::TupleElement(float floatVal)
{
    dataType = FLOAT;
    f = floatVal;
}
