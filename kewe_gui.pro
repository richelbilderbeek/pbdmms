CONFIG += debug_and_release
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../RibiLibraries/Qwt.pri)

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


SOURCES += \
  kewe_qtmain.cpp \
  kewe_qtdialog.cpp \
  kewe_individual.cpp \
  kewe_parameters.cpp \
  kewe_results.cpp \
  kewe_SES.cpp \
  kewe_simulation.cpp

HEADERS  += \
  kewe_qtdialog.h \
  kewe_individual.h \
  kewe_parameters.h \
  kewe_results.h \
  kewe_SES.h \
  kewe_simulation.h

FORMS    += kewe_qtdialog.ui

include(../SurfacePlotter/QtSurfacePlotWidget.pri)

unix:!macx{
  # Linux only
  message("Console application, built for Linux")
  message(Host name: $$QMAKE_HOST.name)
  QMAKE_CXX = g++-5
  QMAKE_LINK = g++-5
  QMAKE_CC = gcc-5
  # -Weffc++ does not play well with Qt
  QMAKE_CXXFLAGS += -Wall -Wextra -std=c++14 -Werror
}


RESOURCES += \
    kewe.qrc

# Prevent Qt for failing with this error:
# qrc_[*].cpp:400:44: error: ‘qInitResources_[*]__init_variable__’ defined but not used
# [*]: the resource filename
QMAKE_CXXFLAGS += -Wno-unused-variable

# Boost.Graph tutorial
#include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

# Boost.Graph
#LIBS += -lboost_graph
