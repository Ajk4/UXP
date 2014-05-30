#include <iostream>
#include "TupleMatcher.h"
#include "TuplePattern.h"
#include <cstring>
int main(int argc, char **argv) {

	for(int i = 0; i < argc; ++i) {
		std::cout << argv[i] << std::endl;
	}

	unsigned char tab[] = "krotka";
	//loat i =13.25;
	//memcpy(tab, &i, 4);
	//conversionUnion.integer  = 0xFA;
	//float x =conversionUnion.floatVariable;
	//std::cout <<"wysłano x " << x << std::endl;
	TupleMatcher *match;
	match = new TupleMatcher(1);
	if(match->CheckString(tab,"Krotka" , 0))
		std::cout << "prawda " <<std::endl;


	/*unsigned char testchar[] = {0x00, 0x00, 0x01, 0x04};
	
	TupleMatcher *match;
	match = new TupleMatcher(1);
	if(match-> CheckInteger(testchar, 4, TuplePattern::GT))
		std::cout<< "prawda"<<std::endl;*/
	return 0;
}
