#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <memory>
#include <list>
#include <regex>
#include <iostream>

/**
 * @brief Structure for storing the requests
 */
typedef struct request_t{
    const std::string host;
    const std::string path;
    const std::string method;
    const std::string authuser;
    const int timestamp;
    const int statusCode;
    const int size;

    /**
     * @brief Basic constructor
     *
     * @param inputHost
     * @param inputPath
     * @param inputMethod
     * @param inputAuthuser
     * @param inputTimestamp
     * @param inputStatusCode
     * @param
     * @param inputPath
     * @param inputMethod
     * @param inputAuthuser
     * @param inputTimestamp
     * @param inputStatusCode
     * @param inputSize
     */
    request_t(std::string inputHost, std::string inputPath, std::string inputMethod, std::string inputAuthuser, int inputTimestamp, int inputStatusCode, int inputSize)
        : host(inputHost)
          , path(inputPath)
          , method(inputMethod)
          , authuser(inputAuthuser)
          , timestamp(inputTimestamp)
          , statusCode(inputStatusCode)
          , size(inputSize)
    {}

    /*
     * @brief Convertion to string
     *
     * @return stirng equivalent to the request
     */
    std::string toString(){
        return host + " " + authuser + " " +  method + " " + path +" " + std::to_string(timestamp);
    }

    /**
     * @brief Extracting section from the path
     *
     * @return section
     */
    std::string getSection(){
        std::regex re("(/[^/]*).*");
        std::smatch match;
        std::string section;
        if (std::regex_search(path, match, re) && match.size() > 0) {
            section = match.str(1);
        } else {
            /* std::cout << "Error parsing regex !" << path << std::endl; */
            return "";
        }
        return section;
    }

} request;

using requestList = std::list<request>;
using requestListPtr = std::shared_ptr<requestList>;

#endif
