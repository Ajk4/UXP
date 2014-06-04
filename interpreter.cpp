#include <sstream>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <string>
#include "Tuple.h"

using namespace std;

int argc;
char **argv;

int pos = 1;

Tuple tuple;

bool isFinish(){
    return pos >= argc;
}

void parseTupleElementType(){
    pos++;
    char* type = argv[pos];
    std::cout << "Type: " << type << std::cout;

    pos++;
    char* rawValue = argv[pos];
    stringstream ss;
    ss << rawValue;
    cout << "RawValue: " << rawValue << std::cout;

    if(strcmp(type, "i") == 0) {
        int intValue;
        ss >> intValue;
        tuple.append(intValue);
    } else if(strcmp(type, "s") == 0) {
        string strValue = string(rawValue);
        tuple.append(strValue);
    } else if(strcmp(type, "f") == 0) {
        float floatValue;
        ss >> floatValue ;
        tuple.append(floatValue);
    }
}

int handleSend() {
    std::cout << "handleSend"  << std::endl;

    while(!isFinish()){
        parseTupleElementType();
    }


    return 0;
}

int handleRead() {
    std::cout << "handleRead"  << std::endl;
    return 0;
}

int handleInput() {
    std::cout << "handleInput"  << std::endl;
    return 0;
}


int main(int _argc, char **_argv) {
    argc = _argc;
    argv = _argv;

    if(argc < 1){
        std::cout << "Usage: (send|read|input)" << std::endl;
        return 1;
    }

    char* operation = argv[1];

    if(strcmp(operation, "send") == 0) {
        return handleSend();
    } else if(strcmp(operation, "read") == 0) {
        return handleRead();
    } else if(strcmp(operation, "input") == 0) {
        return handleInput();
    } else {
        std::cout << "Usage: (send|read|input)" << std::endl;
        return 1;
    }
}
