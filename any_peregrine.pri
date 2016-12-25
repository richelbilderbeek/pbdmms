# C++14
CONFIG += c++14
QMAKE_CXX = g++
QMAKE_LINK = g++
QMAKE_CC = gcc
QMAKE_CXXFLAGS += -std=c++14

# Debug and release mode
CONFIG += debug_and_release

# In release mode, define NDEBUG
CONFIG(release, debug|release) {

  DEFINES += NDEBUG
}

# No Qt
QT -= core gui
