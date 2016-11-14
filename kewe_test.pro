CONFIG += console debug_and_release
CONFIG -= app_bundle
QT -= gui
QT += core
TEMPLATE = app

CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}


SOURCES += \
  kewe_main_test.cpp \
  kewe_test.cpp \
  kewe_SES.cpp \
  kewe_random.cpp \
  kewe_individual_test.cpp \
  kewe_individual.cpp \
  kewe_SES_test.cpp \
  kewe_parameters.cpp \
  kewe_results.cpp \
  kewe_simulation.cpp

# Testing facilities need no R, but the shared code does
# Use g++5 to find boost::unit_test::ut_detail::normalize_test_case_name (will give segmentation fault with g++-4.8)
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
