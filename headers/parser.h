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

        /**
         * @brief Pointer to the backend
         */
        std::weak_ptr<Backend> mBackendPtr;

        /**
         * @brief input stream
         */
        std::ifstream mInputStream;

        /**
         * @brief boolean indicating whether the input in read from stdin
         */
        bool mReadFromStdin = false;

    public:
        /**
         * @brief Basic constructor
         *
         * @param backend pointer to the backend
         * @param filename name of the file to open (stdin if empty)
         */
        Parser(std::weak_ptr<Backend> backend, std::string filename = "");

        /**
         * @brief Read a line from the input stream
         *
         * @return a line of log
         */
        std::string readLogLine();

        /**
         * @brief Get the state of the application
         *
         * @return true if the application is still running, false otherwise
         */
        bool appIsRunning();

        /**
         * @brief Get the a boolean indicating whether the parsing should be paused
         *
         * @return true if the parser needs to be paused, false otherwise
         */
        bool parsingPaused();

        /**
         * @brief Get new request and add it to the mRequestList
         */
        void getNextRequest();

        /**
         * @brief Run the parser
         */
        void run();
};

#endif
