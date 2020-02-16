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

class Application {
    private:
        std::shared_ptr<Backend> mBackendPtr;
        Dashboard mDashboard;
        std::vector<Parser> mParserList;

    public:
        Application(std::string filename = "", unsigned int timewindow = 10, unsigned int alertThreshold = 10, bool isInteractive = true);
        void run();
};

#endif
