#include "backend.h"

Backend::Backend(unsigned int timewindow)
    : mTimeWindow(timewindow)
{}

void Backend::insertRequest(request req){
    std::lock_guard<std::mutex> lock(mRequestsMutex);

    // Insert in the datastructure and initialize currentBuffer if needed
    if (mRequests.empty()){
        mRequests[req.timestamp].push_back(req);
        mCurrentBuffer = buffer(mRequests.begin(), 1);
    } else {
        // Increase buffer size if a key is added and the timestamp is within the time window interval
        if ((req.timestamp - getStartTime() > 0)
                && (req.timestamp - getStartTime() < mTimeWindow)
                && (mRequests.find(req.timestamp) == mRequests.end())) {
            mCurrentBuffer.second++;
        }
        mRequests[req.timestamp].push_back(req);
    }

}

Backend::buffer Backend::getRequestBuffer(){
    std::lock_guard<std::mutex> lock(mRequestsMutex);
    return (mCurrentBuffer);
}

bool Backend::bufIsEmpty(){
    std::lock_guard<std::mutex> lock(mRequestsMutex);
    return mRequests.empty();
}

long unsigned int Backend::getStartTime(){
    return mCurrentBuffer.first->first;
}

unsigned int Backend::getBufferSize(){
    std::lock_guard<std::mutex> lock(mRequestsMutex);
    auto current = mCurrentBuffer.first;
    unsigned int res = 0;
    for (unsigned int i(0); i < mCurrentBuffer.second; i++){
        res += current->second.size();
        current++;
    }
    return res;
}

void Backend::updateWindowSize(){
    long unsigned int timestamp = getStartTime();
    unsigned int res = 0;
    auto itr = mCurrentBuffer.first;
    while ((itr->first < mTimeWindow + timestamp) && (itr != mRequests.end()) && (res < 10)){
        itr++;
        res++;
    }
    mCurrentBuffer.second = res;
}

void Backend::slideTimeWindow(bool forward){
    std::lock_guard<std::mutex> lock(mRequestsMutex);
    if (forward) {
        mCurrentBuffer.first++;
        if (mCurrentBuffer.first == mRequests.end()) {
            mCurrentBuffer.first--;
        }
    } else {
        if (mCurrentBuffer.first != mRequests.begin()) {
            mCurrentBuffer.first--;
        }
    }
    updateWindowSize();
}


const unsigned int Backend::getTimeWindow(){
    return mTimeWindow;
}
