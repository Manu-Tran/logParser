#include "dashboard.h"

bool volatile KeepRunning = true;

void intHandler(int dummy) {
    KeepRunning = false;
}

Dashboard::Dashboard(std::weak_ptr<Backend> backend)
    : mBackendPtr(backend){
    initscr();
    getmaxyx(stdscr,mRows,mCols);		/* get the number of rows and columns */
    endwin();
}



void Dashboard::launchInteractiveUi(){
    WINDOW *stats;
    WINDOW *informations;
    WINDOW *alerts;
    WINDOW *footer;
    WINDOW *requests;

    initscr();

    int offset = 0;
    // Position of the separation between the windows
    mVSep = int(mCols*3/4);
    mHSep = int(mRows*2/3);

    keypad(stdscr, TRUE);
    cbreak();
    refresh();
    noecho();


    stats = createWindow(mHSep-1, mVSep, 0, 0, "Statistics");
    informations = createWindow(mRows-1, mCols-mVSep, 0, mVSep, "Informations");
    alerts = createWindow(mRows-mHSep, mVSep, mHSep-1, 0, "Alerts");
    footer = createWindow(1, mCols, mRows-1, 0, "", false);
    mvwprintw(footer, 0, 5, "TAB: Switch to requests / r: refresh / q:quit / j,k scroll / h,l backward, forward");

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    displayInformation(informations);
    displayStatistics(stats, offset);
    displayAlerts(alerts);

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
                    offset = 0;
                }
                else {
                    destroyWindow(alerts);
                    destroyWindow(stats);
                    requests = createWindow(mRows-1, mVSep, 0, 0, "Requests");
                    displayRequests(requests, offset);
                    offset = 0;
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
                else {
                    offset = std::max(offset-1, 0);
                    displayStatistics(stats, offset);
                }
                break;
            case 'j':
                if (showRequests){
                    offset++;
                    displayRequests(requests, offset);
                }
                else {
                    offset++;
                    displayStatistics(stats, offset);
                }
                break;
            case 'h':
                mBackendPtr.lock()->slideTimeWindow(false);
                if (showRequests){
                    displayRequests(requests, offset);
                }
                else {
                    displayStatistics(stats, offset);
                }
                break;
            case 'l':
                mBackendPtr.lock()->slideTimeWindow(true);
                if (showRequests){
                    displayRequests(requests, offset);
                }
                else {
                    displayStatistics(stats, offset);
                }
                fetchAlert();
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
        if (!showRequests) {
            displayStatistics(stats, offset);
            displayAlerts(alerts);
        }
        displayInformation(informations);
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
    if (mBackendPtr.lock()->bufIsEmpty()) return;
    unsigned int sizeBuffer = mBackendPtr.lock()->getBufferSize();

    // Number of columns that can be printed
    unsigned int windowSize = mRows-3;

    // Estimate if the offset goes beyond the buffer
    if (windowSize + offset > sizeBuffer) {
        offset = std::max((int)(sizeBuffer-windowSize), 0);
    }

    // Number of request printed
    unsigned int count = 0;

    // Number of request skipped
    unsigned int skipCount = 0;

    // Including the element pointed by the end iterator
    auto limit = requestBuffer.second;
    limit++;

    for (auto itr(requestBuffer.first); itr != limit; itr++ ){
    /* for (unsigned int i(0); i < requestBuffer.second; i++){ */
        requestList currentList = itr->second;
        for (auto itr(currentList.begin()); itr != currentList.end(); itr++){
            // Skip the first requests (offset)
            if (skipCount >= offset) {
                deleteLine(win, mVSep,count+1);
                mvwprintw(win,count+1, 1, (*itr).toString().c_str());
                count++;
            } else {
                skipCount++;
            }
            if (count >= windowSize) break;
        }
        if (count >= windowSize) break;
    }
    // Delete the rest of the buffer
    if (count < windowSize) {
        while (count < windowSize){
            deleteLine(win, mVSep, count+1);
            count++;
        }
    }
    wrefresh(win);
}

void Dashboard::deleteLine(WINDOW * win, int cols, int indexLine){
    mvwprintw(win,indexLine, 1, std::string(cols-2, ' ').c_str());
}

void Dashboard::displayInformation(WINDOW *win){
    std::shared_ptr<Backend> backend = mBackendPtr.lock();

    // Get timestamp range
    long unsigned int startTimestamp = backend->getStartTime();
    unsigned int windowSize = backend->getTimeWindow();
    double meanRequests = backend->getMeanRequets();

    std::ostringstream streamObj;
    // Set Fixed -Point Notation
    streamObj << std::fixed;
    // Set precision to 2 digits
    streamObj << std::setprecision(2);
    //Add double to stream
    streamObj << meanRequests;

    std::string meanRequestsPrint = streamObj.str();
    std::string printTimestamp = "["+std::to_string(startTimestamp) + ";" + std::to_string(startTimestamp+windowSize) +"]";

    // Get number of requests
    unsigned int nbRequests = backend->getBufferSize();

    // Reset the buffer
    for (int count(0); count < mRows-3; count++)
        mvwprintw(win, count+1, 1, std::string(mCols-mVSep-2, ' ').c_str());

    // Printing informations
    mvwprintw(win, 1, 1, "Timestamp range : ");
    mvwprintw(win, 2, 1, printTimestamp.c_str());
    mvwprintw(win, 4, 1, (std::string("Nb of requests : ")+std::to_string(nbRequests)).c_str());
    mvwprintw(win, 6, 1, "Mean nb of requests : ");
    mvwprintw(win, 7, 1, meanRequestsPrint.c_str());

    wrefresh(win);
}

void Dashboard::displayStatistics(WINDOW * win, int &offset){
    std::shared_ptr<Backend> backend = mBackendPtr.lock();
    unsigned int offsetTemp = offset;
    std::vector<std::pair<unsigned int, std::string>> mostHits = backend->getMostHits(mHSep, offsetTemp);
    offset = offsetTemp;
    unsigned int count = 0;
    for (auto itr(mostHits.rbegin()); itr != mostHits.rend(); itr++){
        count++;
        deleteLine(win, mVSep, count);
        mvwprintw(win, count, 1, (std::to_string(itr->first)).c_str());
        mvwprintw(win, count, 5, (itr->second).c_str());
    }
    // Delete the rest of the buffer
    if (count < mHSep) {
        while (count < mHSep-3){
            deleteLine(win, mVSep, count+1);
            count++;
        }
    }
    wrefresh(win);
}

void Dashboard::fetchAlert(){
    std::shared_ptr<Backend> backend = mBackendPtr.lock();
    std::string alert = backend->evaluateAlertState();
    if (alert != "") {
        mAlertConsole.push_back(alert);
    }
}

void Dashboard::displayAlerts(WINDOW * win){
    unsigned int count = 0;
    for (auto itr(mAlertConsole.rbegin()); itr != mAlertConsole.rend(); itr++){
        if (count >= mRows-mHSep-2) break;
        deleteLine(win, mVSep, count+1);
        mvwprintw(win, count+1, 1, itr->c_str());
        count++;
    }
    wrefresh(win);
}

void Dashboard::launchUi(){

    signal(SIGINT, intHandler);

    WINDOW *stats;
    WINDOW *informations;
    WINDOW *alerts;
    WINDOW *footer;

    initscr();

    // Position of the separation between the windows
    mVSep = int(mCols*3/4);
    mHSep = int(mRows*2/3);

    cbreak();
    refresh();
    noecho();


    stats = createWindow(mHSep-1, mVSep, 0, 0, "Statistics");
    informations = createWindow(mRows-1, mCols-mVSep, 0, mVSep, "Informations");
    alerts = createWindow(mRows-mHSep, mVSep, mHSep-1, 0, "Alerts");
    footer = createWindow(1, mCols, mRows-1, 0, "", false);
    /* mvwprintw(footer, 0, 5, "TAB: Switch to requests / r: refresh / q:quit / j,k scroll / h,l backward, forward"); */
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    wrefresh(footer);

    while (KeepRunning){
        if (mBackendPtr.lock()->refresh){
            fetchAlert();
            int offset = 0;
            mBackendPtr.lock()->refresh = false;
            displayStatistics(stats, offset);
            displayAlerts(alerts);
            displayInformation(informations);
        } else  {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            mBackendPtr.lock()->slideTimeWindow(true);
            fetchAlert();
        }
    }


    delwin(stats);
    delwin(informations);
    delwin(alerts);
    delwin(footer);
    endwin();
}

