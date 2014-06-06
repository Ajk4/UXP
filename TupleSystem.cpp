#include "TupleSystem.h"
#include <cstdio>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <sys/time.h>
#include <sys/select.h>

void TupleSystem::lindaOpen(const char *name)
{
    int fds[2];
    if(pipe(fds))
    {
	std::cout<< "error pipe" <<std::endl;
    }

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
    fd_set set;
    FD_ZERO(&set);
    FD_SET(tupleRecvFD, &set);
    struct timeval TimeOut;
    Tuple *tupleAddr;
    int rv;
    
    pattern->setOperationType(TuplePattern::INPUT);
    matcher->putPattern(pattern);
    TimeOut.tv_sec = 0;
    TimeOut.tv_usec = timeout*1000000;
    rv = select(tupleRecvFD+1, &set, NULL, NULL, &TimeOut);
    if(rv == -1)
    {matcher->timeoutOccured();std::cout<< "err"<<std::endl; return 0;}/* an error accured */
    else if(rv == 0)
    {matcher->timeoutOccured();std::cout<< "time"<<std::endl; return 0;} /* a timeout occured */
    else
      read(tupleRecvFD, &tupleAddr, sizeof(tupleAddr));
    std::cout<< "good"<<std::endl;
    *output = tupleAddr;
    return 1;
}

int TupleSystem::lindaRead(TuplePattern *pattern, int timeout, Tuple **output)
{
    fd_set set;
    FD_ZERO(&set);
    FD_SET(tupleRecvFD, &set);
    struct timeval TimeOut;
    Tuple *tupleAddr;
    int rv;
    
    pattern->setOperationType(TuplePattern::READ);
    matcher->putPattern(pattern);
    TimeOut.tv_sec = 0;
    TimeOut.tv_usec = timeout*1000000;
    std::cout<< "select"<<std::endl;
    rv = select(tupleRecvFD+1, &set, NULL, NULL, &TimeOut);
    if(rv == -1)
    {matcher->timeoutOccured();std::cout<< "err"<<std::endl; return 0;}/* an error accured */
    else if(rv == 0)
    {matcher->timeoutOccured();std::cout<< "time"<<std::endl;return 0;} /* a timeout occured */
    else
      read(tupleRecvFD, &tupleAddr, sizeof(tupleAddr));
    *output = tupleAddr;
    std::cout<< "good"<<std::endl;
    return 1;
}
