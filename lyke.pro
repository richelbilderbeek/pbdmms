include(lyke.pri)

HEADERS += \
    pbd_helper.h

SOURCES += \
    pbd_helper.cpp \
    lyke_main.cpp

CONFIG += console debug_and_release
CONFIG -= app_bundle
QT += core
QT -= gui
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
