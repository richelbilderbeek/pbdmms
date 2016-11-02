CONFIG += console debug_and_release
CONFIG -= app_bundle
QT -= core gui
TEMPLATE = app

CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}


SOURCES += \
  ribi_main_test.cpp \
  kewe_test.cpp \
  kewe_SES.cpp \
    kewe_random.cpp \
    kewe_individual_test.cpp \
    kewe_individual.cpp \
    kewe_globals.cpp \
    kewe_SES_test.cpp \
    kewe_parameters.cpp


# Must use g++-4.8 and C++0x due to compatibility with R
#QMAKE_CXX = g++-4.8
#QMAKE_LINK = g++-4.8
#QMAKE_CC = gcc-4.8
#QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++11
#SOURCES += /usr/include/boost/test/impl/unit_test_suite.ipp
#HEADERS += /usr/include/boost/test/impl/unit_test_suite.ipp

# Testing facilities need no R, but the shared code does
# Use g++5 to find boost::unit_test::ut_detail::normalize_test_case_name (will give segmentation fault with g++-4.8)
QMAKE_CXX = g++-4.8
QMAKE_LINK = g++-4.8
QMAKE_CC = gcc-4.8
QMAKE_CXXFLAGS += -std=c++11

# Boost.Test
LIBS += -lboost_unit_test_framework

# gcov
QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
LIBS += -lgcov

HEADERS += \
    kewe_random.h \
    kewe_individual.h \
    kewe_globals.h \
    kewe_SES.h \
    kewe_parameters.h
