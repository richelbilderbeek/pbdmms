
CONFIG += console debug_and_release
CONFIG -= app_bundle
QT -= core gui
TEMPLATE = app

CONFIG(release, debug|release) {

  DEFINES += NDEBUG

  # gprof
  QMAKE_CXXFLAGS += -pg
  QMAKE_LFLAGS += -pg
}

CONFIG(debug, debug|release) {

  # gcov
  QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
  LIBS += -lgcov

  # UBSAN
  QMAKE_CXXFLAGS += -fsanitize=undefined
  QMAKE_LFLAGS += -fsanitize=undefined
  LIBS += -lubsan
}

SOURCES += jobo_main.cpp \
    jobo_helper.cpp \
    jobo_individual.cpp \
    jobo_parameters.cpp \
    jobo_simulation.cpp \
    jobo_output.cpp

HEADERS += \
    jobo_helper.h \
    jobo_individual.h \
    jobo_parameters.h \
    jobo_simulation.h \
    jobo_output.h

QMAKE_CXX = g++-5
QMAKE_LINK = g++-5
QMAKE_CC = gcc-5
QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++14

# Boost.Test
LIBS += -lboost_unit_test_framework

# Boost.Graph and GraphViz
LIBS += -lboost_graph

include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)
