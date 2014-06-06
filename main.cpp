#include <iostream>
#include "TuplePattern.h"
#include "TupleSystem.h"
#include <cstring>
#include <mutex>
#include <unistd.h>

std::mutex mtx;
int i = 1;
void funkcja1(const int id);
void funkcja2(const int id);
void funkcja3(const int id);
void funkcja4(const int id);
void funkcja5(const int id);
TupleSystem* sys;
int main(int argc, char **argv) {

	for(int i = 0; i < argc; ++i) {
		std::cout << argv[i] << std::endl;
	}
	/*pid_t pid;
	pid = fork();
	if(pid==0)
	{
	    mtx.lock();
	    int j = i++;
	    mtx.unlock();
	    funkcja1(1);
	    return 0;
	}
	
	pid = fork();
	if(pid==0)
	{
	    mtx.lock();
	    int j = i++;
	    mtx.unlock();
	    funkcja2(2);
	    return 0;
	}
	
	pid = fork();
	if(pid==0)
	{
	    mtx.lock();
	    int j = i++;
	    mtx.unlock();
	    funkcja3(3);
	    return 0;
	}
	
	pid = fork();
	if(pid==0)
	{
	    mtx.lock();
	    int j = i++;
	    mtx.unlock();
	    funkcja4(4);
	    return 0;
	}
	
	funkcja5(5);*/
	
	
	/*int u = 4;
	unsigned char tab[114];
	std::string a = "raz";
	std::string b = "raz";
	Tuple*  tuple;
	tuple = new Tuple();
	tuple->append((std::string)"abc");
	tuple->append(2);
	tuple->append((float)3.3);
	tuple->append((float)3.3);
	
	TuplePattern* pat = new TuplePattern();
	pat->appendString(pat->LE, (std::string)"abc");
    pat->appendInt(pat->ANY, 100);
    pat->appendFloat(pat->ANY, 0);
    pat->appendFloat(pat->GT, 3);
	
	TupleSystem* sys = new TupleSystem();
	sys->lindaOpen((const char*)"radek1");
	sys->lindaOutput(tuple);
	if(sys->lindaInput(pat, 1, &tuple)==1)
	  std::cout<<"znaleziono krotke"<<std::endl;
	if(sys->lindaRead(pat, 1, &tuple)==1)
	  std::cout<<"znaleziono krotke"<<std::endl;
	TupleMatcher *match;*/
	sys = new TupleSystem();
    sys->lindaOpen((const char*)"radek1");
	funkcja1(1);
	funkcja1(1);
	funkcja2(2);
	return 0;
}


void funkcja1(const int id)
{
  
  Tuple*  tuple;
    tuple = new Tuple();
    tuple->append((std::string)"abc");
    tuple->append(2);
    tuple->append((float)3.3);
    tuple->append((float)3.3);
    mtx.lock();
    std::cout<< "proces " << id << " wrzuca krotke zlozona z: \t(s)abc \t(i)2 \t(f)3.3 \t(f)3.3"<<std::endl; 
    mtx.unlock();
    for(int i = 0; i<10; i++)
    sys->lindaOutput(tuple);
}

void funkcja2(const int id)
{

    Tuple* result;
    while(1)
    {
    TuplePattern* pat = new TuplePattern();
    pat->appendString(pat->LE, (std::string)"abc");
    pat->appendInt(pat->ANY, 100);
    pat->appendFloat(pat->ANY, 0);
    pat->appendFloat(pat->GT, 3);
    mtx.lock();
    std::cout<< "proces " << id << " chce dostac krotke zlozona z: \t(s)abc \t(i)* \t(f)* \t(f)>3"<<std::endl; 
    mtx.unlock();
    if(sys->lindaRead(pat, 1, &result)==1)
    {
      mtx.lock();
      std::cout<<"proces "<< id<<" znalazl krotke"<<std::endl;
      mtx.unlock();
    }
    sleep(1);
  }
}

void funkcja3(const int id)
{
  
}

void funkcja4(const int id)
{
  
}

void funkcja5(const int id)
{
  
}
