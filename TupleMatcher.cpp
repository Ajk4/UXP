#include "TupleMatcher.h"
#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>

TupleMatcher::TupleMatcher(int tupleSendFD)
{
	this->tupleSendFD = tupleSendFD;
	pattern = NULL;
	tuple = NULL;
}



int TupleMatcher::match(unsigned char *binaryTuple)
{
	int i;
	char type;
	bool CheckResult = true;
	tuple = new Tuple();
	mtx.lock();
	if( pattern == NULL ) 
	{mtx.unlock();return -1;}
	
	/*rozpoczyna proces sprawdzania*/
	/*pomijam TTL*/
	binaryTuple +=4;
	for(i = 0; i< TUPLE_ELEMENTS; i++)
	{

		memcpy(&type, binaryTuple,1);
		binaryTuple += 1;
		if(pattern->elements[i] == NULL) break;
		else if(pattern->elements[i]->dataType == type)
		{
			if(type == UNKNOWN ) break;
				switch(type)
				{
					case(STRING) 	: CheckResult = CheckString(binaryTuple, pattern->elements[i]->str, pattern->elements[i]->relOP); break; 
					case(INT)	: CheckResult = CheckInteger(binaryTuple, pattern->elements[i]->i, pattern->elements[i]->relOP); break;	
					case(FLOAT)	: CheckResult = CheckFloat(binaryTuple, pattern->elements[i]->f, pattern->elements[i]->relOP); break;
				}
			if(CheckResult)				
			{
				switch(type)
				{
				  case(STRING)	: tuple->append(pattern->elements[i]->str); break;
					case(INT)	: tuple->append(pattern->elements[i]->i); break;
					case(FLOAT)	: tuple->append(pattern->elements[i]->f); break;
				}
				binaryTuple += MAX_STRING_LEN;
				continue;
			}
		}
		/*Jeśli nastąpil blad w czytaniu krotki*/
		delete tuple;
		mtx.unlock();
		return -1;
			

	}
	/*Trzeba sprawdzic czy tuple albu tuple pattern nie jest dluzsze od drugiego*/
	if(i<TUPLE_ELEMENTS)
	{
	  memcpy(&type, binaryTuple,1);
	  
	  if(!(type == UNKNOWN && pattern->elements[i] == NULL)) 
	  {
		delete tuple;
		mtx.unlock();
		return -1; 
	    
	  }
	}
	/*pozytywne znalezienie tuple do wzorca*/
	write(tupleSendFD, &tuple, sizeof(tuple));
	if(pattern->operation == TuplePattern::opType::READ) 	
	{
	  pattern = NULL;
	  mtx.unlock();
	  return 1;
	}
	pattern = NULL;
	mtx.unlock();
	return 0;
	
}


void TupleMatcher::putPattern(TuplePattern *pattern)
{
	
	mtx.lock();
	this->pattern = pattern;
	mtx.unlock();
}

void TupleMatcher::timeoutOccured(void)
{
	mtx.lock();
	pattern = NULL;
	mtx.unlock();

}

bool TupleMatcher::CheckInteger(unsigned char *binaryInt, int intPattern, int relOp)
{
	int integer = 0;
	int CompareResult;
	memcpy(&integer, binaryInt, 4);
	CompareResult = CompareIntegers(integer, intPattern);
	if(RelationResult(CompareResult, relOp))
	{
		return true;
	}
	return false;
}


bool TupleMatcher::CheckFloat(unsigned char *binaryFloat, int floatPattern, int relOp)
{
	int CompareResult;
	float floatResult;
	memcpy(&floatResult, binaryFloat, 4);
	
	CompareResult = CompareFloats(floatResult, floatPattern);
	
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
		if(character == '\0') break;
		str += character;
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
	if( newString > pattern ) 	return TuplePattern::GT;
	else if( newString < pattern) 	return TuplePattern::LT;
	else 				return TuplePattern::EQ;
}

int TupleMatcher::CompareIntegers(int newInteger, int pattern)
{
	if ( newInteger > pattern ) 	return TuplePattern::GT;
	else if( newInteger < pattern ) return TuplePattern::LT;
	else 				return TuplePattern::EQ;
}

int TupleMatcher::CompareFloats(float newInteger, float pattern)
{
	if ( newInteger > pattern ) 		return TuplePattern::GT;
	else if ( newInteger < pattern ) 	return TuplePattern::LT;
	else 					return TuplePattern::EQ;
}
