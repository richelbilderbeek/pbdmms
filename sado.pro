#include(any_profiling.pri)

# C++14
QMAKE_CXX = g++-5
QMAKE_LINK = g++-5
QMAKE_CC = gcc-5
QMAKE_CXXFLAGS += -std=c++14
# -Weffc++ goes bad with Qt
QMAKE_CXXFLAGS += -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic

# Debug and release mode
CONFIG += debug_and_release

# In release mode, turn on profiling
CONFIG(release, debug|release) {

  DEFINES += NDEBUG

  # gprof
  QMAKE_CXXFLAGS += -pg
  QMAKE_LFLAGS += -pg

}

# In debug mode, turn on gcov and UBSAN
CONFIG(debug, debug|release) {

  # gcov
  QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
  LIBS += -lgcov

  # UBSAN
  QMAKE_CXXFLAGS += -fsanitize=undefined
  QMAKE_LFLAGS += -fsanitize=undefined
  LIBS += -lubsan

  # Only in debug mode, a warning is an error
  QMAKE_CXXFLAGS += -Werror
}

# Qt needed for hiostogram to png
QT += core gui widgets

include(sado.pri)
#include(kewe.pri)
#include(pbd.pri)
#include(histogram_to_png.pri)
#include(../gausser/gausser.pri)

# Boost.Graph
#LIBS += -lboost_graph
#include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

#include(../SurfacePlotter/QtSurfacePlotWidget.pri)

SOURCES += sado_main.cpp
