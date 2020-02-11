#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <ncurses.h>
#include <string>
#include <panel.h>
#include <unistd.h>


class Dashboard {
    private:
        int mCols;
        int mRows;
        bool showRequests = false;


        void destroyWindow(WINDOW *win);
        void displayRequests();
        WINDOW * createWindow(int rows, int cols, int offsetRows, int offsetCols, std::string title = "", bool hasBox = true);


    public:
        Dashboard();
        void launchUi();
};

#endif
