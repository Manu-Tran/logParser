#include "application.h"

Application::Application(std::string filename, unsigned int timewindow, unsigned int alertThreshold, bool isInteractive)
  : mBackendPtr(std::make_shared<Backend>(timewindow, alertThreshold))
  , mDashboard(mBackendPtr)
{
    mIsInteractive = isInteractive;
    mParserList.push_back(Parser(mBackendPtr, filename));
}

void Application::run(){
    std::vector<std::thread> threadList;
    for (long unsigned int i(0); i < mParserList.size(); i++){
        threadList.push_back(std::thread(&Parser::run, &mParserList[i]));
    }
    if (mIsInteractive) {
        mDashboard.launchInteractiveUi();
    } else {
        mDashboard.launchUi();
    }
    mBackendPtr->isRunning=false;
    for (long unsigned int i(0); i < threadList.size(); i++){
        threadList[i].join();
    }
}
