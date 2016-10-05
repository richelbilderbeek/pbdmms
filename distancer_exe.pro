
CONFIG += console debug_and_release
CONFIG -= app_bundle
QT -= core gui
TEMPLATE = app

CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}

# Must use g++-4.8 and C++11 due to compatibility with R
QMAKE_CXX = g++-4.8
QMAKE_LINK = g++-4.8
QMAKE_CC = gcc-4.8
QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++0x

# Is that so? R compiles the C++11 files itself, doesn't it?
#QMAKE_CXX = g++-5
#QMAKE_LINK = g++-5
#QMAKE_CC = gcc-5
#QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++11

include(distancer.pri)
include(../../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)
SOURCES += main.cpp

# Boost.Graph and GraphViz, only needed in tests???
LIBS += -lboost_graph
