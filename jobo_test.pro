CONFIG += console debug_and_release
CONFIG -= app_bundle
QT -= core gui
TEMPLATE = app

CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}

#include(distancer.pri)
#include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

#HEADERS += \
#    distancer_population_factory.h

SOURCES += \
    jobo_main_test.cpp \
    jobo_helper.cpp

# Must use g++-4.8 and C++0x due to compatibility with R
#QMAKE_CXX = g++-4.8
#QMAKE_LINK = g++-4.8
#QMAKE_CC = gcc-4.8
#QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++11
#SOURCES += /usr/include/boost/test/impl/unit_test_suite.ipp
#HEADERS += /usr/include/boost/test/impl/unit_test_suite.ipp

# Testing facilities need no R, but the shared code does

# Boost.Test

# gcov
INCLUDEPATH += C:\local\boost_1_62_0
#LIBS += -LC:\local\boost_1_62_0\libs -lboost_unit_test_framework

HEADERS += \
    jobo_helper.h


