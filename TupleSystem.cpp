#include "TupleSystem.h"

void TupleSystem::lindaOpen(const char *name)
{
    Matcher *matcher = NULL;// TODO
    this->pump = new TuplePump(name, matcher);
    this->pump->start();
}

void TupleSystem::lindaClose()
{
    this->pump->stop();
    delete matcher;
    delete pump;
}

void TupleSystem::lindaOutput(Tuple *tuple)
{
    pump->putTuple(tuple);
}

int TupleSystem::lindaInput(TuplePattern *pattern, int timeout, Tuple *output)
{
    matcher->putPattern(patter);
    // TODO brakuje mechanizmu odbioru
}

int TupleSystem::lindaRead(TuplePattern *pattern, int timeout, Tuple *output)
{
    matcher->putPattern(patter);
    // TODO brakuje mechanizmu odbioru
}
