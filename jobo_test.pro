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
    jobo_output.cpp \
    jobo_individuals.cpp \
    jobo_individual.cpp \
    jobo_individual_test.cpp \
    jobo_test_parameters.cpp

HEADERS += \
    jobo_helper.h \
    jobo_parameters.h \
    jobo_simulation.h \
    jobo_output.h \
    jobo_individuals.h \
    jobo_individual.h \
    jobo_individual_test.h \
    jobo_test_parameters.h


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
  contains(QMAKE_HOST.name,pc-157-103) {
    message("Host is student computer")
    QMAKE_CXX = g++-4.8
    QMAKE_LINK = g++-4.8
    QMAKE_CC = gcc-4.8
    QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++11
  }
  !contains(QMAKE_HOST.name,pc-157-103) {
    message("Host is not student computer")
    QMAKE_CXX = g++-5
    QMAKE_LINK = g++-5
    QMAKE_CC = gcc-5
    QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++11
  }

  # Boost.Test
  LIBS += -lboost_unit_test_framework

  # gcov
  QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
  LIBS += -lgcov

  # Boost.Graph and GraphViz, only needed in tests
  LIBS += -lboost_graph
}
