CONFIG += debug_and_release
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += ribi_qtmain.cpp

CONFIG(release, debug|release) {
  DEFINES += NDEBUG
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

include(ribi.pri)
include(pbd.pri)
include(ribi_gui.pri)
include(jkr.pri)
include(../RibiLibraries/Qwt.pri)
include(../SurfacePlotter/QtSurfacePlotWidget.pri)
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

# C++14
QMAKE_CXX = g++-5
QMAKE_LINK = g++-5
QMAKE_CC = gcc-5
# Qt does not go well with -Weffc++
QMAKE_CXXFLAGS += -Wall -Wextra -Werror -std=c++14

# Boost.Graph and GraphViz
LIBS += -lboost_graph

# Prevent Qt for failing with this error:
# qrc_[*].cpp:400:44: error: ‘qInitResources_[*]__init_variable__’ defined but not used
# [*]: the resource filename
QMAKE_CXXFLAGS += -Wno-unused-variable
