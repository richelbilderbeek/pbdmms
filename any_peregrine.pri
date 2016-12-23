# C++14
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

# Boost
DEFINES += BOOST_GRAPH_TUTORIAL_NO_GRAPHVIZ
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

# Boost.Graph and GraphViz
LIBS += -lboost_graph

# No Qt
QT -= core gui
