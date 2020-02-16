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

/**
 * @brief Front end of the application using ncurses
 */
class Dashboard {

    private:

        /**
         * @brief Pointer to the backend
         */
        std::weak_ptr<Backend> mBackendPtr;

        /**
         * @brief number of columns in the terminal
         */
        int mCols;

        /**
         * @brief number of rows in the terminal
         */
        int mRows;

        /**
         * @brief position of the vertical separation
         */
        int mVSep;

        /**
         * @brief position of the horizontal separation
         */
        int mHSep;

        /**
         * @brief history of the alert console
         */
        std::vector<std::string> mAlertConsole;

        /**
         * @brief trigger to switch between layouts
         */
        bool showRequests = false;

        /**
         * @brief helper function that deletes a widnow
         *
         * @param windows to delete
         */
        void destroyWindow(WINDOW *win);


        /**
         * @brief helper function to create a window
         *
         * @param rows number of rows of the window
         * @param cols number of cols of the window
         * @param offsetRows base vertical position of the window
         * @param offsetCols base horizontal position of the window
         * @param title title of the window
         * @param hasBox boolean indicating that box must be drawn
         *
         * @return return the drawn window
         */
        WINDOW * createWindow(int rows, int cols, int offsetRows, int offsetCols, std::string title = "", bool hasBox = true);

        /**
         * @brief helper function to delete a line in a window
         *
         * @param win window to modify
         * @param cols width of the line
         * @param indexRow index of the line
         */
        void deleteLine(WINDOW * win, int cols, int indexRow);

        /**
         * @brief Function that updates the alert console
         */
        void fetchAlert();

        /**
         * @brief Display the information section
         *
         * @param win window to draw on
         */
        void displayInformation(WINDOW *win);

        /**
         * @brief Display the statistics section
         *
         * @param win window to draw on
         * @param offset scroll offset
         */
        void displayStatistics(WINDOW * win, int &offset);

        /**
         * @brief Display the requests section
         *
         * @param win window to draw on
         * @param offset scroll offset
         */
        void displayRequests(WINDOW *win, int &offset);

        /**
         * @brief Display the requests alerts
         *
         * @param win window to draw on
         * @param offset scroll offset
         */
        void displayAlerts(WINDOW * win);


    public:
        /**
         * @brief Basic constructor
         *
         * @param backend pointer to the backend
         */
        Dashboard(std::weak_ptr<Backend> backend);

        /**
         * @brief launch the interactive front end
         */
        void launchInteractiveUi();

        /**
         * @brief launch the non interactive front end
         */
        void launchUi();
};

#endif
