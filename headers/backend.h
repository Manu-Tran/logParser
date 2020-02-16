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
#include <deque>

class Backend {
    // Type definitions
    public:
        // Data structure representing the current buffer (2 iterators)
        using mapIterator = std::map<long unsigned int, requestList>::iterator;
        using buffer = std::pair<mapIterator, mapIterator>;

    private:
        std::mutex mRequestsMutex;

        // Period over which the mean requests per seconds is computed (in seconds)
        unsigned int mAlertMeanPeriod = 120;

        // Maximum timestamp buffering
        unsigned int mMaxTimestampBuffering = 120;

        // Boolean showing if alert is currently occuring
        bool mAlertState = false;

        int mNbRequestInPeriod = 0;


        // Every request earlier than that will be forgotten
        mapIterator mLastTimestampAlert;

        unsigned int mTimeWindow;
        unsigned int mAlertThreshold = 10;
        Backend::buffer mCurrentBuffer;
        std::map<long unsigned int, requestList> mRequests;
        std::multimap<unsigned int, std::string> mMostHits;
        std::unordered_map<std::string, unsigned int> mSectionCount;

        void increaseCountInMap(std::string section);
        void decreaseCountInMap(std::string section);
        void updateRequestCountAlert();

    public:
        // Boolean to communicate the terminaison of the program to the parsers
        std::atomic<bool> isRunning = true;
        std::atomic<bool> refresh = true;

        // Allow to pause the parsers in order to limit the buffer size
        std::atomic<bool> pauseParsing = false;

        Backend(unsigned int timewindow = 10, unsigned int alertThreshold = 10);
        bool bufIsEmpty();
        void insertRequest(request req);

        unsigned int getBufferSize();
        long unsigned int getStartTime();
        long unsigned int getEndTime();
        unsigned int getTimeWindow();
        double getMeanRequets();
        Backend::buffer getRequestBuffer();

        std::vector<std::pair<unsigned int, std::string>> getMostHits(unsigned int nbSections, unsigned int &offset);

        std::string evaluateAlertState();
        void slideTimeWindow(bool forward = false);




        };

#endif
