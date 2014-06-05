#include "TupleSystem.h"

void TupleSystem::lindaOpen(const char *name)
{
    this->matcher = NULL;//TODO
    this->pump = new TuplePump(name, this->matcher);
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
    matcher->putPattern(pattern);
    // TODO brakuje mechanizmu odbioru
}

int TupleSystem::lindaRead(TuplePattern *pattern, int timeout, Tuple *output)
{
    matcher->putPattern(pattern);
    // TODO brakuje mechanizmu odbioru
}
