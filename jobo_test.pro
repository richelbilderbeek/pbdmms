CONFIG += console debug_and_release
CONFIG -= app_bundle
QT -= core gui
TEMPLATE = app

CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}

SOURCES += \
    jobo_main_test.cpp \
    jobo_helper.cpp \
    jobo_parameters.cpp \
    jobo_simulation.cpp \
    jobo_output.cpp

HEADERS += \
    jobo_helper.h \
    jobo_parameters.h \
    jobo_simulation.h \
    jobo_output.h


win32 {
  INCLUDEPATH += C:\local\boost_1_62_0
  # Boost.Test
  #LIBS += -LC:\local\boost_1_62_0\libs -lboost_unit_test_framework
}

unix:!macx{
  # GNU/Linux

  QMAKE_CXX = g++-4.8
  QMAKE_LINK = g++-4.8
  QMAKE_CC = gcc-4.8
  QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++11

  # Boost.Test
  LIBS += -lboost_unit_test_framework

  # gcov
  QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
  LIBS += -lgcov

  # Boost.Graph and GraphViz, only needed in tests
  LIBS += -lboost_graph
}
