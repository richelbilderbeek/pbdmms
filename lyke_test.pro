CONFIG += console debug_and_release
CONFIG -= app_bundle
QT += core
QT -= gui
TEMPLATE = app

CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}

message(Host name: $$QMAKE_HOST.name)
contains(QMAKE_HOST.name,pc-157-106) {
  message("Host is student computer")
  QMAKE_CXX = g++-4.8
  QMAKE_LINK = g++-4.8
  QMAKE_CC = gcc-4.8
  # -Weffc++ does not play well with Qt
  QMAKE_CXXFLAGS += -Wall -Wextra -Werror -std=c++1y
}
!contains(QMAKE_HOST.name,pc-157-106) {
  message("Host is not student computer")
  QMAKE_CXX = g++-5
  QMAKE_LINK = g++-5
  QMAKE_CC = gcc-5
  # -Weffc++ does not play well with Qt
  QMAKE_CXXFLAGS += -Wall -Wextra -Werror -std=c++14
}

# gcov
QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
LIBS += -lgcov

include(lyke.pri)

HEADERS += \
    pbd_helper.h

SOURCES += \
    lyke_main_test.cpp \
    pbd_helper_test.cpp \
    pbd_helper.cpp

# gcov
QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
LIBS += -lgcov

# Boost.Test
LIBS += -lboost_unit_test_framework

# Boost.Graph and GraphViz, only needed in tests
LIBS += -lboost_graph

# Prevent Qt for failing with this error:
# qrc_[*].cpp:400:44: error: ‘qInitResources_[*]__init_variable__’ defined but not used
# [*]: the resource filename
QMAKE_CXXFLAGS += -Wno-unused-variable


