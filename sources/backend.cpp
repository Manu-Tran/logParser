#include "backend.h"

Backend::Backend(unsigned int timewindow)
    : mTimeWindow(timewindow)
{}

void Backend::insertRequest(request req){
    std::lock_guard<std::mutex> lock(mRequestsMutex);

    // Insert in the datastructure and initialize currentBuffer if needed
    if (mRequests.empty()){
        mRequests[req.timestamp].push_back(req);
        mCurrentBuffer = buffer(mRequests.begin(), mRequests.begin());
    } else {
        // Increase buffer size if a key is added and the timestamp is within the time window interval
        if ((req.timestamp > getStartTime())
                && (req.timestamp - getStartTime() <= (mTimeWindow))
                && (mRequests.find(req.timestamp) == mRequests.end())) {
            mRequests[req.timestamp].push_back(req);
            mCurrentBuffer.second++;
        } else mRequests[req.timestamp].push_back(req);
    }
    if (req.timestamp >= getStartTime() && ((req.timestamp - getStartTime()) <= mTimeWindow)){
        std::string section = req.getSection();
        if (section != "") increaseCountInMap(section);
        else std::cout << "Error, no section found !" << section << std::endl;
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
    if (!mRequests.empty())
        return mCurrentBuffer.first->first;
    else
        return 0;
}

unsigned int Backend::getBufferSize(){
    std::lock_guard<std::mutex> lock(mRequestsMutex);
    unsigned int res = 0;

    // Include the end iterator
    mCurrentBuffer.second++;
    for (auto itr(mCurrentBuffer.first); itr != mCurrentBuffer.second; itr++){
        res += itr->second.size();
    }
    mCurrentBuffer.second--;
    return res;
}


void Backend::slideTimeWindow(bool forward){
    std::lock_guard<std::mutex> lock(mRequestsMutex);
    if (forward) {
        requestList requestToRemove = mCurrentBuffer.first->second;
        mCurrentBuffer.first++;
        mCurrentBuffer.second++;
        // If out of bound, undo
        if (mCurrentBuffer.second == mRequests.end()) {
            mCurrentBuffer.first--;
            mCurrentBuffer.second--;
        }
        else {
            // Removing from calculations
            for (auto itr(requestToRemove.begin()); itr != requestToRemove.end(); itr++) {
                decreaseCountInMap(itr->getSection());
            }
            bool isInWindow = (mCurrentBuffer.second->first <= mCurrentBuffer.first->first + mTimeWindow);
            if (!isInWindow) mCurrentBuffer.second--;
            else {
                while (mCurrentBuffer.second != mRequests.end() and isInWindow){
                    // Adding requests to calculations
                    requestList requestToAdd = mCurrentBuffer.second->second;
                    for (auto itr(requestToAdd.begin()); itr != requestToAdd.end(); itr++) {
                        increaseCountInMap(itr->getSection());
                    }
                    mCurrentBuffer.second++;
                    if (mCurrentBuffer.second == mRequests.end()) isInWindow = false;
                    else isInWindow = (mCurrentBuffer.second->first <= mCurrentBuffer.first->first + mTimeWindow);
                }
                mCurrentBuffer.second--;
            }
        }
    } else {
        if (mCurrentBuffer.first != mRequests.begin()) {
            mCurrentBuffer.first--;
            // Adding requests to calculations
            requestList requestToAdd = mCurrentBuffer.first->second;
            for (auto itr(requestToAdd.begin()); itr != requestToAdd.end(); itr++) {
                increaseCountInMap(itr->getSection());
            }
            bool isInWindow = (mCurrentBuffer.second->first <= mCurrentBuffer.first->first + mTimeWindow);
            // Removing from calculations
            while (!isInWindow and mCurrentBuffer.first != mCurrentBuffer.second){
                requestList requestToRemove = mCurrentBuffer.second->second;
                for (auto itr(requestToRemove.begin()); itr != requestToRemove.end(); itr++) {
                    decreaseCountInMap(itr->getSection());
                }
                mCurrentBuffer.second--;
                isInWindow = (mCurrentBuffer.second->first <= mCurrentBuffer.first->first + mTimeWindow);

            }
        }
    }
}


 unsigned int Backend::getTimeWindow(){
    return mTimeWindow;
}

void Backend::increaseCountInMap(std::string section){
    auto sectionPtr = (mSectionCount.find(section));
    if (sectionPtr == mSectionCount.end()){  // If the section is not known
        mSectionCount[section] = 1;
        mMostHits.insert(std::pair<unsigned int, std::string>(1, section));
    } else {                                 // If the section is already known
        unsigned int count = sectionPtr->second;

        // return pair of iterators which indicates elements with equivalent keys
        auto candidates = mMostHits.equal_range(count);

        for (auto itr(candidates.first); itr != candidates.second; itr++){
            if (itr->second == section) {
                mMostHits.erase(itr); // amortized constant
                break;
            }
        }
        (sectionPtr->second)++; // Increase the number of occurences

        // Insert again (sorted) O(log(n))
        mMostHits.insert(std::pair<unsigned int, std::string>(count+1, section));
    }
}

void Backend::decreaseCountInMap(std::string section){
    auto sectionPtr = (mSectionCount.find(section));
    if (sectionPtr == mSectionCount.end() or (sectionPtr->second == 0)){  // If the section is not known
        std::cout << "Error, trying to delete an element that is not present in the buffer" << std::endl;
    } else {                                 // If the section is already known
        unsigned int count = sectionPtr->second;

        // return pair of iterators which indicates elements with equivalent keys
        auto candidates = mMostHits.equal_range(count);
        for (auto itr(candidates.first); itr != candidates.second; itr++){
            if (itr->second == section) {
                mMostHits.erase(itr); // amortized constant
                break;
            }
        }
        if (count != 1){
            (sectionPtr->second)--;
            mMostHits.insert(std::pair<unsigned int, std::string>(count-1, section));
        }
    }
}

std::vector<std::pair<unsigned int, std::string>> Backend::getMostHits(unsigned int nbSections, unsigned int &offset){
    std::lock_guard<std::mutex> lock(mRequestsMutex);
    std::vector<std::pair<unsigned int, std::string>> res;

    // Threshold on the offset
    if (nbSections + offset > mMostHits.size()){
        offset =  std::max((int)(mMostHits.size() - nbSections), 0);
    }
    unsigned int count = 0;

    // Taking count of the offset
    auto itr = mMostHits.begin();
    std::advance(itr, offset);

    // Returning the most hit sections
    while (itr != mMostHits.end() and count != nbSections){
        res.push_back(*itr);
        itr++;
        count++;
    }
    return res;
}
