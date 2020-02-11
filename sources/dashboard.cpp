#include "dashboard.h"

Dashboard::Dashboard() {
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
    int vSep = int(mCols*3/4);
    int hSep = int(mRows*2/3);

    keypad(stdscr, TRUE);
    cbreak();
    refresh();
    noecho();


    stats = createWindow(hSep-1, vSep, 0, 0, "Statistics");
    informations = createWindow(mRows-1, mCols-vSep-1, 0, vSep+1, "Informations");
    alerts = createWindow(mRows-hSep-1, vSep, hSep, 0, "Alerts");
    footer = createWindow(1, mCols, mRows-1, 0, "", false);

    int ch;
    while((ch = getch()) != 'q')
    {	switch(ch)
        {
            // case tab : switching to requests panel
            case 9:
                if (showRequests) {
                    destroyWindow(requests);
                    stats = createWindow(hSep-1, vSep, 0, 0, "Statistics");
                    alerts = createWindow(mRows-hSep-1, vSep, hSep, 0, "Alerts");
                }
                else {
                    destroyWindow(alerts);
                    destroyWindow(stats);
                    requests = createWindow(mRows-1, vSep-1, 0, 0, "Requests");
                }
                showRequests = !showRequests;
                break;
            case 'c':
                wclear(alerts);
                wrefresh(alerts);
                break;

            /* case 410: */
            /*     delwin(requests); */
            /*     delwin(stats); */
            /*     delwin(informations); */
            /*     delwin(alerts); */
            /*     delwin(footer); */
            /*     sleep(1); */
            /*     getmaxyx(stdscr,mRows,mCols);		/1* get the number of rows and columns *1/ */
            /*     vSep = int(mCols*3/4); */
            /*     hSep = int(mRows*2/3); */
            /*     mvprintw(24, 0, "Size now %i %i", mRows, mCols); */
            /*     informations = createWindow(mRows-1, mCols-vSep-1, 0, vSep+1, "Informations"); */
            /*     footer = createWindow(1, mCols, mRows-1, 0, "", false); */
            /*     if (showRequests) { */
            /*         requests = createWindow(mRows-1, vSep-1, 0, 0, "Requests"); */
            /*     } */
            /*     else { */
            /*         stats = createWindow(hSep-1, vSep, 0, 0, "Statistics"); */
            /*         alerts = createWindow(mRows-hSep-1, vSep, hSep, 0, "Alerts"); */
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
    mvwprintw(win, 10, 1, title.c_str());
    wrefresh(win);
    return win;
}
