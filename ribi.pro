
CONFIG += console debug_and_release
CONFIG -= app_bundle
QT -= core gui
TEMPLATE = app

CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}

include(ribi.pri)
include(pbd.pri)
SOURCES += ribi_main.cpp

# C++14
QMAKE_CXX = g++-5
QMAKE_LINK = g++-5
QMAKE_CC = gcc-5
QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++14

# gcov
QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
LIBS += -lgcov

# gprof
QMAKE_CXXFLAGS += -pg
QMAKE_LFLAGS += -pg

# Boost
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

# Boost.Graph and GraphViz
LIBS += -lboost_graph

HEADERS += \
    jkr_experiment.h

