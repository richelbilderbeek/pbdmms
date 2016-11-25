CONFIG += console debug_and_release
CONFIG -= app_bundle
QT -= core gui
TEMPLATE = app

#include(ribi.pri)
#include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

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

include(jobo.pri)

SOURCES += \
    jobo_individual_test.cpp \
    jobo_parameters_test.cpp \
    jobo_simulation_test.cpp \
    jobo_output_test.cpp \
    jobo_jkr_adapters_test.cpp \

SOURCES += jobo_main_test.cpp

win32 {
  # Windows
  message(compiling on Windows)
  CONFIG += c++14

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
  QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++14

  # Boost.Test
  LIBS += -lboost_unit_test_framework


  # Boost.Graph and GraphViz, only needed in tests
  LIBS += -lboost_graph
}
