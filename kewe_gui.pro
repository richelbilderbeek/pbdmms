CONFIG += debug_and_release
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
  kewe_qtmain.cpp \
  kewe_qtdialog.cpp \
  kewe_individual.cpp \
  kewe_parameters.cpp \
  kewe_random.cpp \
  kewe_results.cpp \
  kewe_SES.cpp \
  kewe_simulation.cpp

HEADERS  += \
  kewe_qtdialog.h \
  kewe_individual.h \
  kewe_parameters.h \
  kewe_random.h \
  kewe_results.h \
  kewe_SES.h \
  kewe_simulation.h

FORMS    += kewe_qtdialog.ui

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


RESOURCES += \
    kewe.qrc
