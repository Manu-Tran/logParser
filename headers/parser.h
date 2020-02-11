#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <fstream>
#include <filesystem>
#include <iostream>

/**
 * @brief Object that parses the input stream
 */
class Parser {
    private:
        std::ifstream mInputStream;
        bool mReadFromStdin = false;

    public:
        Parser();
        Parser(std::string filename);
        std::string readline();

};

#endif
