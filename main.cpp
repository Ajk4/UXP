#include <iostream>
#include "TupleMatcher.h"
#include "TuplePattern.h"
union {
	int integer;
	float floatVariable;
} conversionUnion;

int main(int argc, char **argv) {

	for(int i = 0; i < argc; ++i) {
		std::cout << argv[i] << std::endl;
	}

	unsigned char tab[4]  = { 0x0,0x0,0x0, 0xFA};
	conversionUnion.integer  = 0xFA;
	float x =conversionUnion.floatVariable;
	std::cout <<"wysłano x " << x << std::endl;
	TupleMatcher *match;
	match = new TupleMatcher(1);
	match->CheckFloat(tab, 1.23, 0);


	/*unsigned char testchar[] = {0x00, 0x00, 0x01, 0x04};
	
	TupleMatcher *match;
	match = new TupleMatcher(1);
	if(match-> CheckInteger(testchar, 4, TuplePattern::GT))
		std::cout<< "prawda"<<std::endl;*/
	return 0;
}
