CONFIG += debug_and_release
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

include(../RibiLibraries/Qwt.pri)

include(lyke.pri)

SOURCES += \
  pbd_helper.cpp \
  lyke_qtmain.cpp \
  lyke_qtdialog.cpp

HEADERS  += \
  pbd_helper.h \
  lyke_qtdialog.h

FORMS    += lyke_qtdialog.ui

include(../SurfacePlotter/QtSurfacePlotWidget.pri)

CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}

unix:!macx{
  # Linux only
  message("Console application, built for Linux")
  message(Host name: $$QMAKE_HOST.name)
  QMAKE_CXX = g++-5
  QMAKE_LINK = g++-5
  QMAKE_CC = gcc-5
  # -Weffc++ does not play well with Qt
  QMAKE_CXXFLAGS += -Wall -Wextra -std=c++14 -Werror

  # gcov
  QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
  LIBS += -lgcov
}
