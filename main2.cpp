#include <iostream>
#include "TuplePattern.h"
#include "TupleSystem.h"
#include <cstring>
#include <mutex>
#include <unistd.h>

std::mutex mtx;

int main(int argc, char **argv) {
    if(fork()){
        TupleSystem sys;
        sys.lindaOpen("main2");

        Tuple t;
        t.append(2.0f);
        t.append("ke");

        sys.lindaOutput(&t);
        sys.lindaClose();
    }
    if(fork()){
        TupleSystem sys;
        sys.lindaOpen("main2");

        Tuple t;
        t.append(2.0f);
        t.append("ke");
        t.append(33);

        sys.lindaOutput(&t);
        sys.lindaClose();
    }
    if(fork()){
        TupleSystem sys;
        sys.lindaOpen("main2");

        Tuple t;
        t.append("wat");
        t.append("ke");
        t.append(33);

        sys.lindaOutput(&t);
        sys.lindaClose();
    }


    {
        TupleSystem sys;
        sys.lindaOpen("main2");

        TuplePattern p;
        p.appendString(TuplePattern::ANY);
        p.appendString(TuplePattern::ANY);
        p.appendInt(TuplePattern::ANY);

        Tuple *output = NULL;

        sys.lindaRead(&p, 5, &output);
        sys.lindaClose();
    }

    {
        TupleSystem sys;
        sys.lindaOpen("main2");

        TuplePattern p;
        p.appendFloat(TuplePattern::ANY);
        p.appendString(TuplePattern::ANY);

        Tuple *output = NULL;

        sys.lindaRead(&p, 5, &output);
        sys.lindaClose();
    }

}
