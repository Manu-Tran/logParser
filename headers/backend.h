#ifndef BACKEND_H
#define BACKEND_H

#include "request.h"
#include <iostream>
#include <mutex>
#include <list>
#include <iterator>
#include <atomic>
#include <map>
#include <string>

class Backend {
    // Type definitions
    public:
        // Data structure representing the current buffer (2 iterators)
        using mapIterator = std::map<long unsigned int, requestList>::iterator;
        using buffer = std::pair<mapIterator, mapIterator>;

    private:
        std::mutex mRequestsMutex;
        unsigned int mTimeWindow;
        Backend::buffer mCurrentBuffer;
        std::map<long unsigned int, requestList> mRequests;
        std::multimap<unsigned int, std::string> mMostHits;
        std::unordered_map<std::string, unsigned int> mSectionCount;

        void increaseCountInMap(std::string section);
        void decreaseCountInMap(std::string section);

    public:
        std::atomic<bool> isRunning = true;

        Backend(unsigned int timewindow = 10);
        bool bufIsEmpty();
        void insertRequest(request req);

        unsigned int getBufferSize();
        long unsigned int getStartTime();
        unsigned int getTimeWindow();
        Backend::buffer getRequestBuffer();
        std::vector<std::pair<unsigned int, std::string>> getMostHits(unsigned int nbSections, unsigned int &offset);


        void slideTimeWindow(bool forward = false);




};

#endif
