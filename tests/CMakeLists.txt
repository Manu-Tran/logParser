find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

find_package(Threads REQUIRED)
include_directories(${THREADS_INCLUDE_DIR})

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})


file(
    GLOB
    headers
    ${CMAKE_SOURCE_DIR}/headers/*.h
    )

add_executable (runTests test.cpp)

target_include_directories(runTests PUBLIC ${CMAKE_SOURCE_DIR}/headers)

target_link_libraries(runTests ${CURSES_LIBRARIES})
target_link_libraries (runTests ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)

