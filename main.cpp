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
int main(int argc, char **argv) {

	for(int i = 0; i < argc; ++i) {
		std::cout << argv[i] << std::endl;
	}
	if(fork())
	{
	    funkcja1(1);
	    return 0;
	}
	
	if(fork())
	{
	    funkcja2(2);
	    return 0;
	}
	
	if(fork())
	{
	    funkcja3(3);
	    return 0;
	}
	
	    funkcja4(4);
	    return 0;
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
	/*sys = new TupleSystem();
    sys->lindaOpen((const char*)"radek1");
	funkcja1(1);
	funkcja1(1);
	funkcja2(2);
	return 0;*/
}


void funkcja1(const int id)
{
TupleSystem* sys;
sys = new TupleSystem();
sys->lindaOpen("uxp1a");
  
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

        sleep(4);

    }
}

void funkcja2(const int id)
{
TupleSystem* sys;
sys = new TupleSystem();
sys->lindaOpen("uxp1a");
  

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
    
TupleSystem* sys;
sys = new TupleSystem();
sys->lindaOpen("uxp1a");
  
    Tuple*  tuple;
    Tuple* result;
    TuplePattern* pat;
    mtx.lock();
    while(1)
    {
        tuple = new Tuple();
        std::cout<< "Proces " << id << " wrzuca krotke zlozona z: ";
        tuple->append(5); 
            std::cout<<"\t(i) 5";
        tuple->append(10.0f);
            std::cout<<"\t(f) 10";
        std::cout<< std::endl;
        mtx.unlock();
        sys->lindaOutput(tuple);

        pat = new TuplePattern();
        mtx.lock();
        std::cout<< "Proces " << id << " chce dostac krotke zlozona z:";
        pat->appendString(pat->EQ, (std::string)"radek"); 
            std::cout<<"\t(s) == radek";
        pat->appendFloat(pat->ANY, 0);
            std::cout<<"\t(f) *";
        std::cout << std::endl;
        mtx.unlock();
        
         if(sys->lindaInput(pat, 1, &result)==1)
        {
              mtx.lock();
              std::cout<<"proces "<< id<<" znalazl krotke"<<std::endl;
              mtx.unlock();
        }
        else
        {
              mtx.lock();
              std::cout<<"proces "<< id<<"nie znalazl krotki"<<std::endl;
              mtx.unlock();
        }
        sleep(5);
        }
}

void funkcja4(const int id)
{
    std::cout<< "proces 4" <<std::endl;
TupleSystem* sys;
sys = new TupleSystem();
sys->lindaOpen("uxp1a");
  
    Tuple*  tuple;
    Tuple*  result;
    TuplePattern* pat;
    while(1)
    {
        pat = new TuplePattern();
        tuple = new Tuple();
        mtx.lock();
        std::cout<< "Proces " << id << " chce dostac krotke zlozona z:";
        pat->appendInt(pat->LE, 6); 
            std::cout<<"\t(i) <= 6";
        pat->appendFloat(pat->ANY, 0);
            std::cout<<"\t(f) *";
        std::cout << std::endl;
        mtx.unlock();
        if(sys->lindaInput(pat, 1, &result)==1)
        {
              mtx.lock();
              std::cout<<"proces "<< id<<" znalazl krotke"<<std::endl;
              mtx.unlock();
        }
        else
        {
              mtx.lock();
              std::cout<<"proces "<< id<<"nie znalazl krotki"<<std::endl;
              mtx.unlock();
        }
        
        mtx.lock();
        std::cout<< "Proces " << id << " wrzuca krotke zlozona z: ";
        tuple->append((std::string)"radek"); 
            std::cout<<"\t(s) radek";
        tuple->append(999.0f);
            std::cout<<"\t(f) 999.0";
        std::cout<< std::endl;
        mtx.unlock();
        sys->lindaOutput(tuple);
        delete tuple;
        delete pat;
        sleep(1);
    }
}

void funkcja5(const int id)
{
  
}
