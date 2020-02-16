# About

LogParser is terminal utilitary to examine and monitor a http server throught the log file.

# Table of contents

  * [Setup](#Setup)
  * [Requirements](#Requirements)
  * [Installation](#install)
  * [Run](#run)
  * [Documentation](#documentation)
  * [Architecture](#architecture)
  * [Overview](#overview)
  * [Structure](#structure)
  * [File Structure](#file-structure)
* [Possible improvements](#possible-improvements)


# Setup

## TL DR;
  Requires :
  - CMake
  - ncurses
  - gtest
  - Doxygen

For interactive mode:
  ```
  cmake .
  make
  ./logParser -f logfile1 -f logfile2 ...
  ```

For non interactive mode:
  ```
  cmake .
  make
  cat logfile | ./logParser
  ```

For generating documentation:
```
doxygen Doxyfile
```

For launching test:
```
./tests/runTests
```


## Requirements :

  The program requires :

  At least **C++11**

  - **CMake** to manage the build proces
- **Ncurses** to manage the user interface (Dashboard)
  - **Google Test**
  - **Doxygen** to generate the documentation

  Logparser has been tested in Linux and will not work on Windows because of the dependency with ncurses.

## Installation :

### Libs for Linux :

It is possible to install the requirement in debian-based distro with :

  **Libs installation**
  ```
  apt-get install cmake
  apt-get install ncurses
  apt-get install doxygen   # to generate doc
  apt-get install libgtest-dev
  ```
  **Google Test installation**
  ```
  cd /usr/src/gtest
  cmake CMakeLists.txt
  make
  cp *.a /usr/lib
  ```


### CMake (for building)
  ```
  cd (project)
  cmake .
  make
  ```

## Run

To run the tests :
```
./tests/runTests
```

To launch in non iteractive mode, just pipe an executable on logParser :
```
${logGeneration} | ./logParser
```

To launch in interactive mode :
```
./logParser -f $FILENAME
```
And then :
- press TAB for switching layout
- press j,k for scrolling
- press h,l for increasing or decreasing the timestamp
- press q to quit

For further help, please use the ```-h``` option.


## Documentation

  To generate the documentation :
  Install doxygen : ```apt-get install doxygen```

  Generation :
  ```
  doxygen Doxyfile
  ```

  To access the documentation : Open ```(project)/html/index.html```

  Most of the documentation is located in the header files.


# Architecture

## Overview

### Global architecture

  This project is composed of three components :
  - The front constituing the interface with the user : Dashboard
  - The back constituing the application logic : Backend
  - The parsers constituing the interface with the log files: Parser

  And Application wraps up those compoenents.

  The front and the parsers run on different threads (one for the front, and one per parsers), allowing the interface to be detached from the I/O.

### General Purpose

  Dashboard :
    - Displays the information and alert received from the backend
    - Handles user input and buffer scrolling

  Backend :
    - Stores the requests and delete useless requests
    - Raises an alert if needed
    - Computes the differents metrics (mean requests, most hits section...)
    - Controls the amout of requests stored
    - Controls the accesss to the shared data structure

  Parser :
    - Parses the input streams
    - Builds the request struct

  Application :
    - Launching the threads and joining them at the end
    - Instentiates the other components


## File structure
  ```
  .
  ├── /sources
  │   ├─ Application.cpp
  │   ├─ Dashboard.cpp
  │   ├─ Backend.cpp
  │   ├─ Parser.cpp
  │   └─ main.cpp
  ├── /headers
  │   ├─ Application.h
  │   ├─ Dashboard.h
  │   ├─ Parser.h
  │   ├─ args.hxx
  │   └─ Backend.h
  ├── /tests
  │   ├─ CMakeLists.txt
  │   └─ test.cpp
  ├─ Doxyfile
  └─ CMakeLists.txt
  ```

# Possible Improvements
  - ** Adding other metrics ** : It would have been posssible to add more metrics such as gettng count of status code over last requests.
  - ** Enabling reverse parsing ** : Allowing the backend to ask the parsers for the first line again
  - ** More robust parsers ** : Being able to build more diverse parsers to parse other type of file than csv, or csv with differents columns.
  - ** Better mutex management ** : Here there is only one mutex protecting the relevant Backend functions. It has the advantage of avoiding deadlocks but can approach a limit in term of scalability if there are multi parsers.
  - ** Better responsivity ** : The frontend does not respond very well to terminal resizing. It would be interesting to detect that kind of changes and refresh.







