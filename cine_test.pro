

CONFIG += console debug_and_release
CONFIG -= app_bundle
QT -= core gui
TEMPLATE = app

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

unix:!macx{
  # Linux only
  message("Console application, built for Linux")
  message(Host name: $$QMAKE_HOST.name)

  CONFIG += c++14
  QMAKE_CXX = g++-5
  QMAKE_LINK = g++-5
  QMAKE_CC = gcc-5
  QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++14

  # Boost.Test
  LIBS += -lboost_unit_test_framework
}

HEADERS += \
    cine_simulation.h \
    cine_landscape.h \
    cine_plot.h \
    cine_individual.h \
    cine_population.h

SOURCES += \
    cine_simulation.cpp \
    ribi_main_test.cpp \
    cine_simulation_test.cpp \
    cine_individual.cpp \
    cine_landscape.cpp \
    cine_plot.cpp \
    cine_population.cpp

# OpenCV
unix {
   CONFIG += link_pkgconfig
   PKGCONFIG += opencv
}
