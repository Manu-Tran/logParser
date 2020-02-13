#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <ncurses.h>
#include <panel.h>
#include <unistd.h>
#include <memory>
#include <string>

#include "backend.h"

class Dashboard {
    private:
        std::weak_ptr<Backend> mBackendPtr;
        int mCols;
        int mRows;
        int mVSep;
        int mHSep;
        bool showRequests = false;


        void destroyWindow(WINDOW *win);
        void displayRequests(WINDOW *win, int &offset);
        WINDOW * createWindow(int rows, int cols, int offsetRows, int offsetCols, std::string title = "", bool hasBox = true);
        void deleteLine(WINDOW * win, int cols, int indexRow);


    public:
        Dashboard(std::weak_ptr<Backend> backend);
        void launchUi();
};

#endif
