#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <iomanip>
#include <ncurses.h>
#include <panel.h>
#include <unistd.h>
#include <memory>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <signal.h>
#include <vector>

#include "backend.h"

class Dashboard {
    private:
        std::weak_ptr<Backend> mBackendPtr;
        int mCols;
        int mRows;
        int mVSep;
        int mHSep;
        std::vector<std::string> mAlertConsole;
        bool showRequests = false;


        void destroyWindow(WINDOW *win);
        WINDOW * createWindow(int rows, int cols, int offsetRows, int offsetCols, std::string title = "", bool hasBox = true);
        void deleteLine(WINDOW * win, int cols, int indexRow);

        void fetchAlert();

        void displayInformation(WINDOW *win);
        void displayStatistics(WINDOW * win, int &offset);
        void displayRequests(WINDOW *win, int &offset);
        void displayAlerts(WINDOW * win);


    public:
        Dashboard(std::weak_ptr<Backend> backend);
        void launchInteractiveUi();
        void launchUi();
};

#endif
