CONFIG += console debug_and_release
CONFIG -= app_bundle
QT -= gui
QT += core
TEMPLATE = app

CONFIG(release, debug|release) {
  DEFINES += NDEBUG
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

# gcov
QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
LIBS += -lgcov

HEADERS += \
    kewe_random.h \
    kewe_individual.h \
    kewe_SES.h \
    kewe_parameters.h \
    kewe_results.h \
    kewe_simulation.h

RESOURCES += \
    kewe.qrc
