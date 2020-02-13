#include "parser.h"

Parser::Parser(std::weak_ptr<Backend> backend, std::string name)
    : mBackendPtr(backend){
    if (name == "") {
        mReadFromStdin = true;
    }
    else if (!std::filesystem::exists(name)) {
        std::cout << "Unable to load file, switching to stdin !" << std::endl;
        mReadFromStdin = true;
    }
    else {
        mInputStream.open(name);
        std::cout << "Readind from file " + name + " with columns : " << readLogLine();
        mReadFromStdin = false;
    }
}

std::string Parser::readLogLine(){
    std::string res = "";
    while (res == "" and appIsRunning()){
        if (mReadFromStdin) {
            std::getline(std::cin, res);
        }
        else {
            std::getline(mInputStream, res);
        }
        if (res == "") sleep(1);
    }
    return res;
}


/**
 * @brief Split the CSV line with ','
 *
 * @param input a line of the CSV file
 *
 * @return vector of values without quotes
 */
std::vector<std::string> parseCSV(std::string input){
    std::vector<std::string> res;

    std::size_t index = 0;
    std::size_t newIndex = input.find(",");
    std::string tmp = (input.substr(index, newIndex-index));
    if (tmp[0] == '"'){
        tmp = tmp.substr(1, tmp.length() -2);
    }
    res.push_back(tmp);

    while (index != std::string::npos){
        index = newIndex;
        newIndex = input.find(",", index+1);

        tmp = (input.substr(index+1, newIndex-index-1));
        if (tmp[0] == '"'){
            tmp = tmp.substr(1, tmp.length() -2);
        }
        /* std::cout << "foo" << tmp << std::endl; */
        res.push_back(tmp);
        /* std::cout << index << " diff " << std::string::npos << std::endl; */
    }
    return res;
}

void Parser::getNextRequest(){
    std::string res = readLogLine();
    /* std::cout << "Printing " << res << std::endl; */
    if (appIsRunning()){
    std::vector<std::string> parsedInput = parseCSV(res);
    std::string path;
    std::string method;

    std::regex re("(.*) (.*) .*");
    std::smatch match;
    if (std::regex_search(parsedInput[4], match, re) && match.size() > 2) {
        method = match.str(1);
        path = match.str(2);
    } else {
        std::cout << "Error parsing regex ! " << std::endl;
        return;
    }

    mBackendPtr.lock()->insertRequest(
            request(
                parsedInput[0],
                path,
                method,
                parsedInput[2],
                std::stoi(parsedInput[3]),
                std::stoi(parsedInput[5]),
                std::stoi(parsedInput[6])
                )
            );
    }
}

void Parser::run(){
    int count = 0;
    while (true and appIsRunning()){
        getNextRequest();
        count++;
    }
}

bool Parser::appIsRunning(){
    return mBackendPtr.lock()->isRunning;
}
