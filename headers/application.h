#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <list>
#include <mutex>
#include <thread>
#include <vector>

#include "dashboard.h"
#include "parser.h"
#include "request.h"
#include "backend.h"

/**
 * @brief Class containing the other component and launching threads
 */
class Application {
    private:
        /**
         * @brief Pointer to the backend.
         *  The backend is seen by the other component through a weak_ptr
         */
        std::shared_ptr<Backend> mBackendPtr;

        /**
         * @brief Front end with ncurses.
         * Will be executed by this thread
         */
        Dashboard mDashboard;

        /**
         * @brief Vector containing all the parsers.
         * Concurrent reading of different streams is possible
         */
        std::vector<Parser> mParserList;

        /**
         * @brief Boolean indicating if the application is interactive
         */
        bool mIsInteractive;

    public:
        /**
         * @brief Contructor
         *
         * @param filename names of the files that will be read (if none, then stdin)
         * @param timewindow size of the sample for stats (in sec)
         * @param alertThreshold mean requests in a 2-min-window needed to trigger an alert
         * @param isInteractive bool indicating whether the application is interactive
         */
        Application(std::vector<std::string> filenames, unsigned int timewindow = 10, unsigned int alertThreshold = 10, bool isInteractive = true);

        /**
         * @brief Launches the thread
         */
        void run();
};

#endif
