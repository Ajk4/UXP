
#include <iostream>

#include "Tuple.h"
#include "TuplePattern.h"
#include "TupleSystem.h"

#include <unistd.h>

int main(int _argc, char **_argv) {
    char *fifoName = _argv[1];

    TupleSystem tupleSystem;
    tupleSystem.lindaOpen(fifoName);

    TuplePattern pattern;
    pattern.appendFloat(TuplePattern::LT, 3);
    pattern.appendString(TuplePattern::ANY);
    pattern.appendInt(TuplePattern::ANY);

    Tuple output;

    while (true){
        std::cout << "Reading tuple" << std::cout;
        tupleSystem.lindaRead(&pattern, 10, &output);
        usleep(5000);
    }

    tupleSystem.lindaClose();

}
