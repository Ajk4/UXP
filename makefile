CC=g++
CFLAGS=-Wall -pedantic -W -pthread -std=c++11

all: interpreter.o main.o Tuple.o TupleMatcher.o TuplePattern.o TuplePump.o TupleSystem.o
	$(CC) $(CFLAGS) main.o Tuple.o TupleMatcher.o TuplePattern.o TuplePump.o TupleSystem.o

TupleSystem.o:
	$(CC) $(CFLAGS) -c TupleSystem.cpp

main.o:
	$(CC) $(CFLAGS) -c main.cpp
	
Tuple.o:
	$(CC) $(CFLAGS) -c Tuple.cpp

TupleMatcher.o:
	$(CC) $(CFLAGS) -c TupleMatcher.cpp
	
TuplePattern.o:
	$(CC) $(CFLAGS) -c TuplePattern.cpp
	
TuplePump.o:
	$(CC) $(CFLAGS) -c TuplePump.cpp
		
clean:
	rm *.o
