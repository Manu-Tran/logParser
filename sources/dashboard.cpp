#include "dashboard.h"

Dashboard::Dashboard(std::weak_ptr<Backend> backend)
    : mBackendPtr(backend){
    initscr();
    getmaxyx(stdscr,mRows,mCols);		/* get the number of rows and columns */
    endwin();
}

void Dashboard::launchUi(){

    WINDOW *stats;
    WINDOW *informations;
    WINDOW *alerts;
    WINDOW *footer;
    WINDOW *requests;

    initscr();

    // Position of the separation between the windows
    int offset = 0;
    mVSep = int(mCols*3/4);
    mHSep = int(mRows*2/3);

    keypad(stdscr, TRUE);
    cbreak();
    refresh();
    noecho();


    stats = createWindow(mHSep-1, mVSep, 0, 0, "Statistics");
    informations = createWindow(mRows-1, mCols-mVSep-1, 0, mVSep+1, "Informations");
    alerts = createWindow(mRows-mHSep, mVSep, mHSep-1, 0, "Alerts");
    footer = createWindow(1, mCols, mRows-1, 0, "", false);
    mvwprintw(footer, 0, 5, "TAB: Switch to requests / r: refresh / q:quit");
    wrefresh(footer);

    int ch;
    while((ch = getch()) != 'q')
    {	switch(ch)
        {
            // case tab : switching to requests panel
            case 9:
                if (showRequests) {
                    destroyWindow(requests);
                    stats = createWindow(mHSep-1, mVSep, 0, 0, "Statistics");
                    alerts = createWindow(mRows-mHSep, mVSep, mHSep-1, 0, "Alerts");
                }
                else {
                    destroyWindow(alerts);
                    destroyWindow(stats);
                    requests = createWindow(mRows-1, mVSep-1, 0, 0, "Requests");
                    displayRequests(requests, offset);
                }
                showRequests = !showRequests;
                break;
            case 'c':
                wclear(alerts);
                wrefresh(alerts);
                break;
            case 'r':
                if (showRequests){
                    displayRequests(requests, offset);
                }
                break;
            case 'k':
                if (showRequests){
                    offset = std::max(offset-1, 0);
                    displayRequests(requests, offset);
                }
                break;
            case 'j':
                if (showRequests){
                    offset++;
                    displayRequests(requests, offset);
                }
                break;
            /* case 410: */
            /*     delwin(requests); */
            /*     delwin(stats); */
            /*     delwin(informations); */
            /*     delwin(alerts); */
            /*     delwin(footer); */
            /*     sleep(1); */
            /*     getmaxyx(stdscr,mRows,mCols);		/1* get the number of rows and columns *1/ */
            /*     mVSep = int(mCols*3/4); */
            /*     mHSep = int(mRows*2/3); */
            /*     mvprintw(24, 0, "Size now %i %i", mRows, mCols); */
            /*     informations = createWindow(mRows-1, mCols-mVSep-1, 0, mVSep+1, "Informations"); */
            /*     footer = createWindow(1, mCols, mRows-1, 0, "", false); */
            /*     if (showRequests) { */
            /*         requests = createWindow(mRows-1, mVSep-1, 0, 0, "Requests"); */
            /*     } */
            /*     else { */
            /*         stats = createWindow(mHSep-1, mVSep, 0, 0, "Statistics"); */
            /*         alerts = createWindow(mRows-mHSep-1, mVSep, mHSep, 0, "Alerts"); */
            /*     } */
            /*     break; */
            default:
                mvprintw(24, 0, "Charcter pressed is = %3d Hopefully it can be printed as '%c'", ch, ch);
                refresh();
                break;
        }
    }

    delwin(requests);
    delwin(stats);
    delwin(informations);
    delwin(alerts);
    delwin(footer);
    endwin();
}

void Dashboard::destroyWindow(WINDOW *win)
{
    wborder(win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(win);
    delwin(win);
}

WINDOW * Dashboard::createWindow(int rows, int cols, int offsetRows, int offsetCols, std::string title, bool hasBox){
    WINDOW * win = newwin(rows, cols, offsetRows, offsetCols);
    if (hasBox) box(win, 0 , 0);
    mvwprintw(win, 0, 1, title.c_str());
    /* mvwprintw(win, 10, 1, title.c_str()); */
    wrefresh(win);
    return win;
}

void Dashboard::displayRequests(WINDOW * win, int &offset){
    Backend::buffer requestBuffer = mBackendPtr.lock()->getRequestBuffer();
    /* wclear(win); */
    /* win = createWindow(mRows-1, mVSep-1, 0, 0, "Requests"); */
    // Number of columns that can be printed
    unsigned int windowSize = mRows-3;

    // Estimate if the offset goes beyond the buffer
    if (windowSize + offset > requestBuffer.second) {
        offset = std::max((int)(requestBuffer.second-windowSize), 0);
    }
    unsigned int nbPrintedRequest = std::min(requestBuffer.second, windowSize);

    // Add the offset to the itérator
    for (int i(0); i < offset; i++) requestBuffer.first++;

    for (int count(nbPrintedRequest); count != 0; count--){
        deleteLine(win, mVSep-1,nbPrintedRequest - count +1 );
        mvwprintw(win,nbPrintedRequest - count +1, 1, (*requestBuffer.first).toString().c_str());
        requestBuffer.first++;
    }
    wrefresh(win);
}

void Dashboard::deleteLine(WINDOW * win, int cols, int indexLine){
    mvwprintw(win,indexLine, 1, std::string(cols-2, ' ').c_str());
}
