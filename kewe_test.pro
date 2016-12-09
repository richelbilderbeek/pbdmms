CONFIG += console debug_and_release
CONFIG -= app_bundle
QT -= gui
QT += core
TEMPLATE = app

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

include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)


SOURCES += \
  kewe_main_test.cpp \
  kewe_test.cpp \
  kewe_SES.cpp \
  kewe_individual_test.cpp \
  kewe_individual.cpp \
  kewe_SES_test.cpp \
  kewe_parameters.cpp \
  kewe_results.cpp \
  kewe_simulation.cpp \
    kewe_simulation_test.cpp \
    kewe_parameters_test.cpp \
    kewe_results_test.cpp

# C++14
CONFIG += c++14
QMAKE_CXX = g++-5
QMAKE_LINK = g++-5
QMAKE_CC = gcc-5
QMAKE_CXXFLAGS += -Wall -Wextra -std=c++14

# Boost.Test
LIBS += -lboost_unit_test_framework

HEADERS += \
    kewe_individual.h \
    kewe_SES.h \
    kewe_parameters.h \
    kewe_results.h \
    kewe_simulation.h

RESOURCES += \
    kewe.qrc

# Prevent Qt for failing with this error:
# qrc_[*].cpp:400:44: error: ‘qInitResources_[*]__init_variable__’ defined but not used
# [*]: the resource filename
QMAKE_CXXFLAGS += -Wno-unused-variable

  # Boost.Graph and GraphViz, only needed in tests
  LIBS += -lboost_graph
