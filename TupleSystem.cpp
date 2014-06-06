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
    struct timeval TimeOut;
    Tuple *tupleAddr;
    int rv;
    
    
    FD_ZERO(&set);
    FD_SET(tupleRecvFD, &set);
    
    pattern->setOperationType(TuplePattern::INPUT);
    matcher->putPattern(pattern);
    
    TimeOut.tv_sec = timeout;
    TimeOut.tv_usec = 0;
    
    rv = select(tupleRecvFD+1, &set, NULL, NULL, &TimeOut);
    if(rv == -1)/* an error accured */
    {
        matcher->timeoutOccured(); 
        return 0;
        
    }
    else if(rv == 0) /* a timeout occured */
    {
        matcher->timeoutOccured();
        return 0;
        
    }
    else
      read(tupleRecvFD, &tupleAddr, sizeof(tupleAddr));
    *output = tupleAddr;
    return 1;
}

int TupleSystem::lindaRead(TuplePattern *pattern, int timeout, Tuple **output)
{
    fd_set set;
    struct timeval TimeOut;
    Tuple *tupleAddr;
    int rv;
    
    
    FD_ZERO(&set);
    FD_SET(tupleRecvFD, &set);
    
    pattern->setOperationType(TuplePattern::INPUT);
    matcher->putPattern(pattern);
    
    TimeOut.tv_sec = timeout;
    TimeOut.tv_usec = 0;
    
    rv = select(tupleRecvFD+1, &set, NULL, NULL, &TimeOut);
    if(rv == -1)/* an error accured */
    {
        matcher->timeoutOccured(); 
        return 0;
        
    }
    else if(rv == 0) /* a timeout occured */
    {
        matcher->timeoutOccured();
        return 0;
        
    }
    else
      read(tupleRecvFD, &tupleAddr, sizeof(tupleAddr));
    *output = tupleAddr;
    return 1;
}
