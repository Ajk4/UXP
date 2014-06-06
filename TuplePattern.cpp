#include "TuplePattern.h"

TuplePattern::TuplePattern() {
	operation = relOp::ANY;
	actualElementIndex = 0;
	for(int i = 0; i < TUPLE_ELEMENTS ; ++i) {
		elements[i] = NULL;
	}
}

TuplePattern::~TuplePattern()
{
	for(int i = 0; i < actualElementIndex ; ++i) {
		delete elements[i];
	}
}

void TuplePattern::setOperationType(int op)
{
    this->operation = op;
}

int TuplePattern::appendString(TuplePattern::relOp op, std::string value)
{
    if(actualElementIndex >= TUPLE_ELEMENTS){
        return OperationCode::TUPLE_PATTERN_FULL;
    } else {
        elements[actualElementIndex] = new TuplePatternElement(op, value);
        actualElementIndex++;
        return OperationCode::OK;
    }
}

int TuplePattern::appendInt(TuplePattern::relOp op, int value)
{
    if(actualElementIndex >= TUPLE_ELEMENTS){
        return OperationCode::TUPLE_PATTERN_FULL;
    } else {
        elements[actualElementIndex] = new TuplePatternElement(op, value);
        actualElementIndex++;
        return OperationCode::OK;
    }
}

int TuplePattern::appendFloat(TuplePattern::relOp op, float value)
{
    if(actualElementIndex >= TUPLE_ELEMENTS){
        return OperationCode::TUPLE_PATTERN_FULL;
    } else {
        elements[actualElementIndex] = new TuplePatternElement(op, value);
        actualElementIndex++;
        return OperationCode::OK;
    }
}

TuplePattern::TuplePatternElement::TuplePatternElement(int relOP, const std::string &s) : TupleElement(s)
{
    this->relOP = relOP;
}


TuplePattern::TuplePatternElement::TuplePatternElement(int relOP, int i) : TupleElement(i)
{
    this->relOP = relOP;
}

TuplePattern::TuplePatternElement::TuplePatternElement(int relOP, float f) : TupleElement(f)
{
    this->relOP = relOP;
}


