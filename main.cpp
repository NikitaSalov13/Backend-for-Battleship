#include <string>
#include <iostream>
#include <sstream>

#include "Field.h"
#include "ParseFile.h"
#include "Commands.h"



int main() {
    while(true) {
        std::string cmd;
        std::getline(std::cin, cmd);

        if(cmd == "exit") {
            return 0;

        }
        if(cmd == "ping") {
            std::cout << "pong" << std::endl;
        }
        else if (cmd == "create master") {
            std::cout<<"ok"<<'\n';
            CreateMaster();
        }
        else if (cmd == "create slave") {
            std::cout<<"ok"<<'\n';
            CreateSlave();
        }
    }
}