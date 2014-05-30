#include "TupleMatcher.h"
#include <string>
#include <iostream>


TupleMatcher::TupleMatcher(int tupleSendFD)
{
	this->tupleSendFD = tupleSendFD;
	pattern = NULL;
	
}



int TupleMatcher::match(unsigned char *binaryTuple)
{
	if( pattern == NULL ) return -1;
	
	return 1;
}


void TupleMatcher::putPattern(TuplePattern *pattern)
{
	
	this->pattern = pattern;
}

void TupleMatcher::timeoutOccured(void)
{


}

bool TupleMatcher::CheckInteger(unsigned char *binaryInt, int intPattern, int relOp)
{
	int integer = 0;
	int CompareResult;	
	for(int i=0; i<4; i++, binaryInt++)
	{
		integer = integer << 8;
		integer = integer | *binaryInt;
	}

	CompareResult = CompareIntegers(integer, intPattern);

	if(RelationResult(CompareResult, relOp))
	{
		return true;
	}
	return false;
}


bool TupleMatcher::CheckFloat(unsigned char *binaryFloat, int floatPattern, int relOp)
{
	union {
		int integer;
		float floatVariable;
	} conversionUnion;

	int NotYetFloat = 0;
	int CompareResult;	
	float floatResult;
	for(int i=0; i<4; i++, binaryFloat++)
	{
		NotYetFloat = NotYetFloat << 8;
		NotYetFloat = NotYetFloat | *binaryFloat;
	}
	std::cout <<"odebrano x " << NotYetFloat << std::endl;
	conversionUnion.integer = NotYetFloat;
	floatResult = conversionUnion.floatVariable;
	std::cout <<"uzyskano Float: " << floatResult<< std::endl;
	CompareResult = CompareIntegers(floatResult, floatPattern);

	if(RelationResult(CompareResult, relOp))
	{
		return true;
	}
	return false;
}

bool TupleMatcher::CheckString(unsigned char *binaryString, std::string stringPattern, int relOp)
{
	char character = 0x00;
	int CompareResult;
	std::string 	str = "";
	for(int i =0; i< MAX_STRING_LEN; i++)
	{
		character = character | *binaryString;
		str += character;
		if(character == '\0') break;
		character = 0x00;
		binaryString++;
	}
	CompareResult = CompareStrings(str, stringPattern);	
	if(RelationResult(CompareResult, relOp)) 
	{
		return true;
	}
	return false;
}	


bool TupleMatcher::RelationResult(int CompareResult, int patternOperator)
{
	switch(patternOperator)
	{
		case TuplePattern::GT: 	if ( CompareResult != TuplePattern::GT ) return false;
					break;
		case TuplePattern::LT: 	if ( CompareResult != TuplePattern::LT ) return false;
					break;
		case TuplePattern::GE:	if ( CompareResult == TuplePattern::LT ) return false;
					break;
		case TuplePattern::LE:	if ( CompareResult == TuplePattern::GT ) return false; 
					break;
		case TuplePattern::EQ:	if ( CompareResult != TuplePattern::EQ ) return false;
					break;
		case TuplePattern::ANY: break; 
	}
	return true;
}


int TupleMatcher::CompareStrings(std::string newString, std::string pattern)
{
	int len;
	if(newString.size() >= newString.size())
		len = pattern.size();
	else
		len = newString.size();

	for(int i = 0; i<len; i++)
	{
		if(newString[i] > pattern[i]) return TuplePattern::GT;
		else if( newString[i] < pattern[i] ) return TuplePattern::LT;
	}
	
	if(newString.size()==pattern.size()) return TuplePattern::EQ;
	else if( newString.size() > pattern.size()) return TuplePattern::GT;
	else return TuplePattern::LT;
}

int TupleMatcher::CompareIntegers(int newInteger, int pattern)
{
	if ( newInteger > pattern ) return TuplePattern::GT;
	else if ( newInteger < pattern ) return TuplePattern::LT;
	else return TuplePattern::EQ;
}

int TupleMatcher::CompareFloats(float newInteger, float pattern)
{
	if ( newInteger > pattern ) return TuplePattern::GT;
	else if ( newInteger < pattern ) return TuplePattern::LT;
	else return TuplePattern::EQ;
}
