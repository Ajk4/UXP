
#include <iostream>

#include "Tuple.h"
#include "TupleSystem.h"

#include <unistd.h>

int main(int _argc, char **_argv) {
    char *fifoName = _argv[1];

    TupleSystem tupleSystem;
    tupleSystem.lindaOpen(fifoName);

    Tuple tuple;
    tuple.append(2.0f);
    tuple.append("asd");
    tuple.append(3);
    while (true){
        std::cout << "Sending tuple" << std::endl;
        tupleSystem.lindaOutput(&tuple);
        usleep(5000);
    }

    tupleSystem.lindaClose();

}
