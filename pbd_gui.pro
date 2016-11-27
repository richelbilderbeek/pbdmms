CONFIG += debug_and_release
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += pbd_qtmain.cpp

CONFIG(release, debug|release) {
  DEFINES += NDEBUG
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

include(pbd.pri)
include(pbd_gui.pri)
include(../RibiLibraries/Qwt.pri)

unix:!macx{
  # Linux only
  message("Console application, built for Linux")
  message(Host name: $$QMAKE_HOST.name)
  QMAKE_CXX = g++-5
  QMAKE_LINK = g++-5
  QMAKE_CC = gcc-5
  # Qt does not go well with -Weffc++
  QMAKE_CXXFLAGS += -Wall -Wextra -Werror -std=c++14
}

