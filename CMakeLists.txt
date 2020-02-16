CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

set(CMAKE_BUILD_TYPE Debug)

Project(logParser)

#======================================================================
# Version
SET(CPACK_PACKAGE_VERSION_MAJOR "0")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "0")

SET(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

#======================================================================
# Common setup

if (CMAKE_VERSION VERSION_LESS "3.1")
    add_definitions(-std=c++17)
else()
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()


set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

#======================================================================
# Executable
add_compile_options(
  )

file (
  GLOB
  sources
  sources/*.cpp
  )

file (
  GLOB
  headers
  headers/*.h
  headers/*.hxx
  )

add_executable(logParser ${sources})

target_compile_features(logParser
  PRIVATE cxx_variadic_templates)

target_include_directories(logParser PRIVATE headers)
target_link_libraries(logParser
  ncurses
  pthread)

enable_testing ()
add_subdirectory(tests)
# add_test (NAME Test
    # COMMAND tests)

# install(PROGRAMS logParser
#   DESTINATION ${CMAKE_SOURCE_DIR})

#======================================================================
# Unit testing

# enable_testing()
# add_subdirectory(lib/googletest)
# include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
# include_directories(${gmock_SOURCE_DIR}/include ${gmock_SOURCE_DIR})

# add_subdirectory(tests)
