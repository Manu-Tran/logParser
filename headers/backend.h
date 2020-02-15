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
        // Data structure representing the current buffer (a start iterator and the length)
        using buffer = std::pair<std::map<long unsigned int, requestList>::iterator, unsigned int>;

    private:
        std::mutex mRequestsMutex;
        const unsigned int mTimeWindow;
        Backend::buffer mCurrentBuffer;
        std::map<long unsigned int, requestList> mRequests;

    public:
        std::atomic<bool> isRunning = true;

        Backend(unsigned int timewindow = 10);
        bool bufIsEmpty();
        void insertRequest(request req);

        unsigned int getBufferSize();
        long unsigned int getStartTime();
        const unsigned int getTimeWindow();
        Backend::buffer getRequestBuffer();
        void updateWindowSize();


        void slideTimeWindow(bool forward = false);




};

#endif
