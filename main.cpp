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
    mtx.lock();
    while(1)
    {
        std::cout<< "Proces " << id << " wrzuca krotke zlozona z: ";
        tuple->append((std::string)"abc");
            std::cout<<"\t(s) abc";
        tuple->append(2); 
            std::cout<<"\t(i) 2";
        tuple->append(3.3f);
            std::cout<<"\t(f) 3.3";
        tuple->append(3.3f); 
            std::cout<<"\t(f) 3.3";
    	tuple->append(3);
    	    std::cout<<"\t(i) 3";
    	tuple->append(4.8f); 
    	    std::cout<<"\t(f) 4.4";
    	tuple->append((std::string)"jeden");
    	    std::cout<<"\t(s) jeden";
    	tuple->append(4);
    	    std::cout<<"\t(i) 4";
    	tuple->append(4.4f); 
    	    std::cout<<"\t(f) 4.4";
    	tuple ->append(1234);
    	    std::cout<<"\t(i) 1234";
        std::cout<< std::endl;
        mtx.unlock();
        
        sys->lindaOutput(tuple);
        sleep(3)
    }
}

void funkcja2(const int id)
{

    Tuple* result;
    TuplePattern* pat;
    while(1)
    {
        pat = new TuplePattern();
        mtx.lock();
        std::cout<< "Proces " << id << " chce dostac krotke zlozona z:";
        pat->appendString(pat->LE, (std::string)"abc"); 
            std::cout<<"\t(s) <= abc";
        pat->appendInt(pat->ANY, 100);
            std::cout<<"\t(i) *";
        pat->appendFloat(pat->ANY, 0);
            std::cout<<"\t(f) *";
        pat->appendFloat(pat->GT, 3);
            std::cout<<"\t(f) >3";
    	pat->appendInt(pat->LE, 4);
            std::cout<<"\t(i) <=4";
    	pat->appendFloat(pat->GT, 4.5);
            std::cout<<"\t(f) >4.5";
    	pat->appendString(pat->ANY, (std::string)"");
    	    std::cout<<"\t(s) *";
    	pat->appendInt(pat->ANY, 0);
            std::cout<<"\t(i) *";
    	pat->appendFloat(pat->GT, 1.0);
            std::cout<<"\t(f) >1.0";
    	pat->appendInt(pat->ANY, 0);
            std::cout<<"\t(i) *";
            std::cout<< ::std::endl;
        mtx.unlock();
        
        if(sys->lindaInput(pat, 1, &result)==1)
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
    Tuple*  tuple;
    Tuple* result;
    tuple = new Tuple();
    mtx.lock();
    while(1)
    {
        std::cout<< "Proces " << id << " wrzuca krotke zlozona z: ";
        tuple->append(5); 
            std::cout<<"\t(i) 5";
        tuple->append(10.0f);
            std::cout<<"\t(f) 10";
        std::cout<< std::endl;
        mtx.unlock();
        sys->lindaOutput(tuple);
        sleep(3)
    }
}

void funkcja4(const int id)
{
    Tuple*  tuple;
    tuple = new Tuple();
    mtx.lock();
    while(1)
    {
        
        std::cout<< "Proces " << id << " wrzuca krotke zlozona z: ";
        tuple->append(5); 
            std::cout<<"\t(i) 5";
        tuple->append(10.0f);
            std::cout<<"\t(f) 10";
        std::cout<< std::endl;
        mtx.unlock();
        sys->lindaOutput(tuple);
        
        sleep(3)
    }
}

void funkcja5(const int id)
{
  
}
