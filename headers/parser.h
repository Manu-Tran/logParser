#ifndef PARSER_H
#define PARSER_H

#include <unistd.h>
#include <string>
#include <fstream>
#include <filesystem>
#include <mutex>
#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <regex>


#include "request.h"
#include "backend.h"

/**
 * @brief Object that parses the input stream
 */
class Parser {
    private:
        std::weak_ptr<Backend> mBackendPtr;
        std::ifstream mInputStream;
        bool mReadFromStdin = false;

    public:
        /* Parser(std::weak_ptr<Backend> backend); */
        Parser(std::weak_ptr<Backend> backend, std::string filename = "");

        /**
         * @brief Read a line from the input stream
         *
         * @return a line of log
         */
        std::string readLogLine();

        bool appIsRunning();

        /**
         * @brief Get new request and add it to the mRequestList
         */
        void getNextRequest();

        void run();
};

#endif
