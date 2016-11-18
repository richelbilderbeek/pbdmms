CONFIG += console debug_and_release
CONFIG -= app_bundle
QT -= core gui
TEMPLATE = app

#include(ribi.pri)
#include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}

include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

SOURCES += \
    ribi_main_test.cpp \
    jobo_parameters.cpp \
    jobo_simulation.cpp \
    jobo_output.cpp \
    jobo_individuals.cpp \
    jobo_individual.cpp \
    jobo_individual_test.cpp \
    jobo_parameters_test.cpp \
    jobo_simulation_test.cpp \
    jobo_output_test.cpp

# jobo_main_test.cpp

HEADERS += \
    jobo_parameters.h \
    jobo_simulation.h \
    jobo_output.h \
    jobo_individuals.h \
    jobo_individual.h \
    jobo_individual_test.h \
    jobo_parameters_test.h \
    jobo_simulation_test.h \
    jobo_output_test.h


win32 {
  # Windows
  message(compiling on Windows)
  CONFIG += c++11

  # High warning level, warnings are errors
  QMAKE_CXXFLAGS += -Wall -W3 -WX

  # Boost.Test
  INCLUDEPATH += C:/local/boost_1_62_0
  LIBS += -LC:/local/boost_1_62_0/lib64-msvc-14.0 -llibboost_unit_test_framework-vc140-mt-gd-1_62
}

unix:!macx{
  # GNU/Linux

  message("Console application, built for Linux")
  message(Host name: $$QMAKE_HOST.name)
  CONFIG += c++17
  QMAKE_CXX = g++-5
  QMAKE_LINK = g++-5
  QMAKE_CC = gcc-5
  QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++17

  # gcov
  QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
  LIBS += -lgcov

  # Boost.Test
  LIBS += -lboost_unit_test_framework


  # Boost.Graph and GraphViz, only needed in tests
  LIBS += -lboost_graph
}
