#include "TupleSystem.h"
#include <cstdio>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void TupleSystem::lindaOpen(const char *name)
{
    int fds[2];
    pipe(fds);

    this->tupleRecvFD = fds[0];

    this->matcher = new TupleMatcher(fds[1]);
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

int TupleSystem::lindaInput(TuplePattern *pattern, int timeout, Tuple **output)
{
    matcher->putPattern(pattern);

    pattern->setOperationType(TuplePattern::INPUT);

    read(this->tupleRecvFD, tupleAddrBuff, ADDR_SIZE);
    Tuple *tupleAddr;
    // kopiuje wskaznik na tuple z bufora
    memcpy(&tupleAddr, tupleAddrBuff, ADDR_SIZE);

    // zapisuje pod wskaznikiem na wskaznik, wskaznik na tuple
    *output = tupleAddr;
    return 0;
}

int TupleSystem::lindaRead(TuplePattern *pattern, int timeout, Tuple **output)
{
    matcher->putPattern(pattern);

    pattern->setOperationType(TuplePattern::READ);

    read(this->tupleRecvFD, tupleAddrBuff, ADDR_SIZE);
    Tuple *tupleAddr;
    memcpy(&tupleAddr, tupleAddrBuff, ADDR_SIZE);

    *output = tupleAddr;
    return 0;
}
