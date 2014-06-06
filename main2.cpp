#include <iostream>
#include "TuplePattern.h"
#include "TupleSystem.h"
#include <cstring>
#include <mutex>
#include <unistd.h>
#include <boost/interprocess/sync/scoped_lock.hpp>

std::mutex mtx;

int main(int argc, char **argv) {
    if(fork()){
        TupleSystem sys;
        sys.lindaOpen("main2");

        Tuple t;
        t.append(2.0f);
        t.append("ke");

        std::cout << "Send tuple [2.0, 'ke']" << std::endl;

        sys.lindaOutput(&t);
        sys.lindaClose();
        return 0;
    }
    if(fork()){
        TupleSystem sys;
        sys.lindaOpen("main2");

        Tuple t;
        t.append(2.0f);
        t.append("ke");
        t.append(33);

        std::cout << "Send tuple [2.0, 'ke', 33]" << std::endl;

        sys.lindaOutput(&t);
        sys.lindaClose();
        return 0;
    }
    if(fork()){
        TupleSystem sys;
        sys.lindaOpen("main2");

        Tuple t;
        t.append("wat");
        t.append("ke");
        t.append(33);

        std::cout << "Send tuple ['wat', 'ke', 33]" << std::endl;

        sys.lindaOutput(&t);
        sys.lindaClose();
        return 0;
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

        std::cout << "Read pattern [STR:*, STR:*, INT:*]" << std::endl;
    }

    {
        TupleSystem sys;
        sys.lindaOpen("main2");

        TuplePattern p;
        p.appendFloat(TuplePattern::ANY);
        p.appendString(TuplePattern::ANY);

        Tuple *output = NULL;

        sys.lindsaRead(&p, 5, &output);
        sys.lindaClose();

        std::cout << "Read pattern [FLO:*, STR:*]" << std::endl;
    }

}
