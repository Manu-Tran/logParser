#include "application.h"

Application::Application()
  : mBackendPtr(std::make_shared<Backend>())
  , mDashboard(mBackendPtr)
{
  mParserList.push_back(Parser(mBackendPtr, "logfile"));
}

void Application::run(){
    std::vector<std::thread> threadList;
    for (long unsigned int i(0); i < mParserList.size(); i++){
        threadList.push_back(std::thread(&Parser::run, &mParserList[i]));
    }
    mDashboard.launchUi();
    mBackendPtr->isRunning=false;
    for (long unsigned int i(0); i < threadList.size(); i++){
        threadList[i].join();
    }
}
