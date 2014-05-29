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

bool TupleMatcher::CheckString(unsigned char *binaryString, std::string stringPattern, int relOp)
{
	char character = 0x00;
	int CompareResult;
	std::string 	str = "";
	bool endStringToken = true;
	for(int i =0; i< MAX_STRING_LEN; i++)
	{
		if(endStringToken)
		{
			character = character | *binaryString;
			str += character;
			if(character == '\0') endStringToken = false;
			character = 0x00;
			binaryString++;
		}
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
