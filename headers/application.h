#ifndef APPLICATION_H
#define APPLICATION_H

#include <memory>
#include <list>

#include "dashboard.h"
#include "parser.h"
#include "request.h"

class Application {
    private:
        Dashboard mDashboard;

    public:
        using RequestList = std::list<Request>;
        /* Application(); */
        void run();
};

#endif
