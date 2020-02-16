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

/**
 * @brief Interface between the parsers and the front
 */
class Backend {
    // Type definitions
    public:
        /**
         * @brief iterator of the requests data structure
         */
        using mapIterator = std::map<long unsigned int, requestList>::iterator;

        /**
         * @brief data structure representing a group of requests (one start iterator and one end iterator)
         */
        using buffer = std::pair<mapIterator, mapIterator>;

    private:
        /**
         * @brief Mutex protecting the shared data structures
         */
        std::mutex mRequestsMutex;

        /**
         * @brief Period over which the mean requests per seconds is computed (in seconds)
         */
        unsigned int mAlertMeanPeriod = 120;

        /**
         * @brief Maximum timestamp buffering (in seconds)
         */
        unsigned int mMaxTimestampBuffering = 120;

        /**
         * @brief Boolean showing if alert is currently occuring
         */
        bool mAlertState = false;

        /**
         * @brief Count of the number of requests within mAlertMeanPeriod from the current time
         */
        int mNbRequestInPeriod = 0;


        /**
         * @brief Every request earlier than that will be forgotten.
         * Allow a limitations of the number of requests stored
         *
         */
        mapIterator mLastTimestampAlert;

        /**
         * @brief size of the sample for the stats computing (in seconds)
         */
        unsigned int mTimeWindow;

        /**
         * @brief mean requests in a 2-min-window needed to trigger an alert
         */
        unsigned int mAlertThreshold = 10;

        /**
         * @brief buffer containing the current sample for stats computing
         */
        Backend::buffer mCurrentBuffer;


        /**
         * @brief Data structure containing transiting requests.
         * It maps a timestamp to a list of request struct
         * Old values are erased.
         */
        std::map<long unsigned int, requestList> mRequests;

        /**
         * @brief Data structure keeping track of the most hits section
         * Mapping a count to a section
         */
        std::multimap<unsigned int, std::string> mMostHits;

        /**
         * @brief Data structure keeping track of the most hits section.
         * Mapping a section to a count.
         */
        std::unordered_map<std::string, unsigned int> mSectionCount;

        /**
         * @brief increments in maps mMostHits and mSectionCount the occurences of section
         * @param section name of the section to increment
         */
        void increaseCountInMap(std::string section);

        /**
         * @brief decrements in maps mMostHits and mSectionCount the occurences of section
         * @param section name of the section to decrement
         */
        void decreaseCountInMap(std::string section);

        /**
         * @brief updates the number of requests counted in for the alert threshold
         */
        void updateRequestCountAlert();

    public:
        /**
         * @brief Boolean to communicate the terminaison of the program to the parsers.
         * It is turned once to false and kill the parsers
         */
        std::atomic<bool> isRunning = true;

        /**
         * @brief Boolean to allow the front to reload in non interactive mode
         */
        std::atomic<bool> refresh = true;

        /**
         * @brief Allow to pause the parsers in order to limit the buffer size
         */
        std::atomic<bool> pauseParsing = false;

        /**
         * @brief Basic constructor
         *
         * @param timewindow size of the sample for the stats computing (in seconds)
         * @param alertThreshold mean requests in a 2-min-window needed to trigger an alert
         */
        Backend(unsigned int timewindow = 10, unsigned int alertThreshold = 10);

        /**
         * @brief Determine the state of the requests buffer
         *
         * @return true if the mRequests is empty, false otherwise
         */
        bool bufIsEmpty();


        /**
         * @brief Add a request to the buffer and updates the backend's state accordingly
         *
         * @param req request to add
         */
        void insertRequest(request req);

        /**
         * @brief Get the number of request in the current buffer
         *
         * @return request count
         */
        unsigned int getBufferSize();

        /**
         * @brief Get the earliest timestamp of the buffer
         *
         * @return timestamp
         */
        long unsigned int getStartTime();


        /**
         * @brief Get the latest timestamp of the buffer
         *
         * @return timestamp
         */
        long unsigned int getEndTime();


        /**
         * @brief Getter for the attribute mTimeWindow
         *
         * @return the attricute mTimeWindow
         */
        unsigned int getTimeWindow();


        /**
         * @brief Get the mean request by using the attribute mNbRequestInPeriod
         *
         * @return mean requests per seconds during the last 2 minutes
         */
        double getMeanRequets();

        /**
         * @brief Get the requests of the last 10s
         *
         * @return a pair of iterator pointing to a portion of mRequests
         */
        Backend::buffer getRequestBuffer();

        /**
         * @brief Get a list of the most hits sections
         *
         * @param nbSections max count of sections to return
         * @param offset offset of the sections to return (allowing scroll in the front)
         *
         * @return a vector of pair <timestamp, section>
         */
        std::vector<std::pair<unsigned int, std::string>> getMostHits(unsigned int nbSections, unsigned int &offset);

        /**
         * @brief Get the alert if one has raised
         *
         * @return an empty string if there is no change in state, else a message describing the alert
         */
        std::string evaluateAlertState();

        /**
         * @brief move the buffer to a later timestamp while updating the backend.
         *
         * @param forward boolean indicating whether to go back in time or forward
         */
        void slideTimeWindow(bool forward = true);
};

#endif
