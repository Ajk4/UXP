#include <iostream>
#include "TupleMatcher.h"
#include "TuplePattern.h"

int main(int argc, char **argv) {

	for(int i = 0; i < argc; ++i) {
		std::cout << argv[i] << std::endl;
	}

	unsigned char testchar[] = "radek";
	
	TupleMatcher *match;
	match = new TupleMatcher(1);
	if(match-> CheckString(testchar, "Jeden", TuplePattern::GT))
		std::cout<< "prawda"<<std::endl;
	return 0;
}
