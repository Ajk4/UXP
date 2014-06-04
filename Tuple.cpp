#include "Tuple.h"


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
