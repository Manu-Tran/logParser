#ifndef REQUEST_H
#define REQUEST_H

#include <string>
#include <memory>
#include <list>

typedef struct request_t{
    const std::string host;
    const std::string path;
    const std::string method;
    const std::string authuser;
    const int timestamp;
    const int statusCode;
    const int size;

    request_t(std::string inputHost, std::string inputPath, std::string inputMethod, std::string inputAuthuser, int inputTimestamp, int inputStatusCode, int inputSize)
        : host(inputHost)
          , path(inputPath)
          , method(inputMethod)
          , authuser(inputAuthuser)
          , timestamp(inputTimestamp)
          , statusCode(inputStatusCode)
          , size(inputSize)
    {}

    std::string toString(){
        return host + " " + authuser + " " +  method + " " + path +" " + std::to_string(timestamp);
    }
} request;

/* using requestPtr = std::shared_ptr<request>; */
using requestList = std::list<request>;
using requestListPtr = std::shared_ptr<requestList>;

#endif
