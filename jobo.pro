
CONFIG += console debug_and_release
CONFIG -= app_bundle
QT -= core gui
TEMPLATE = app

CONFIG(release, debug|release) {
  DEFINES += NDEBUG
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


win32 {
  # Windows
  message(compiling on Windows)
  # Boost.Test

  # High warning level, warnings are errors
  QMAKE_CXXFLAGS += -Wall -W3 -WX

  INCLUDEPATH += C:/local/boost_1_62_0
  #LIBS += "-LC:/local/boost_1_62_0/libs/"
  #LIBS += "-LMyLibraryPath" -lmylib
  #LIBS += -lboost_graph
}

unix:!macx{
  # GNU/Linux
  message(compiling on GNU/Linux)

  message("Console application, built for Linux")
  message(Host name: $$QMAKE_HOST.name)
  QMAKE_CXX = g++-5
  QMAKE_LINK = g++-5
  QMAKE_CC = gcc-5
  QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++14

  # Boost.Test
  LIBS += -lboost_unit_test_framework

  # gcov
  QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
  LIBS += -lgcov

  # Boost.Graph and GraphViz, only needed in tests
  LIBS += -lboost_graph
}

