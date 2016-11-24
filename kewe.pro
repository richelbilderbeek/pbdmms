CONFIG += console debug_and_release
CONFIG -= app_bundle
QT -= gui
QT += core
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

SOURCES += \
  kewe_SES.cpp \
    kewe_random.cpp \
    kewe_individual.cpp \
    kewe_parameters.cpp \
    kewe_results.cpp \
    kewe_simulation.cpp \
    kewe_main.cpp

CONFIG += c++14
QMAKE_CXX = g++-5
QMAKE_LINK = g++-5
QMAKE_CC = gcc-5
QMAKE_CXXFLAGS += -std=c++14

# Boost.Test
LIBS += -lboost_unit_test_framework

HEADERS += \
    kewe_random.h \
    kewe_individual.h \
    kewe_SES.h \
    kewe_parameters.h \
    kewe_results.h \
    kewe_simulation.h \
    jkr_experiment.h

RESOURCES += \
    kewe.qrc
