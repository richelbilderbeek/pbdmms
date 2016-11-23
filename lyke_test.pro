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

HEADERS += \
    pbd_helper.h \
    lyke_individual.h \
    lyke_random.h \
    lyke_utils.h \
    lyke_simulation.h

SOURCES += \
    pbd_helper_test.cpp \
    pbd_helper.cpp \
    kewe_main_test.cpp \
    lyke_individual.cpp \
    lyke_random.cpp \
    lyke_simulation.cpp \
    lyke_utils.cpp \
    lyke_simulation_test.cpp

RESOURCES += \
    lyke.qrc

# gcov
QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
LIBS += -lgcov

# Boost.Test
LIBS += -lboost_unit_test_framework

# Boost.Graph and GraphViz, only needed in tests
LIBS += -lboost_graph


