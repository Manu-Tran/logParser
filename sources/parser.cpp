#include "parser.h"

Parser::Parser(std::string name){
    if (!std::filesystem::exists(name)) {
        std::cout << "Unable to load file, switching to stdin !" << std::endl;
        mReadFromStdin = true;
    }
    else {
        mInputStream.open(name);
        mReadFromStdin = false;
    }
}

Parser::Parser(){
    mReadFromStdin = true;
}

std::string Parser::readline(){
    std::string res;
    if (mReadFromStdin) {
        std::getline(std::cin, res);
    }
    else {
        std::getline(mInputStream, res);
    }
    return res;
}

