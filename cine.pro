TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += cine_source.cpp \
    cine_simulation.cpp \
    cine_landscape.cpp \
    cine_plot.cpp \
    cine_individual.cpp \
    cine_population.cpp \
    cine_parameters.cpp \
    cine_ann.cpp

CONFIG += console debug_and_release
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

# C++14
QMAKE_CXX = g++-5
QMAKE_LINK = g++-5
QMAKE_CC = gcc-5
QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++14

HEADERS += \
    cine_simulation.h \
    cine_landscape.h \
    cine_plot.h \
    cine_individual.h \
    cine_population.h \
    cine_parameters.h \
    cine_ann.h \
    cine_simulation.hpp

#LIBS += -L/usr/local/lib -lshark

## OpenCV
#unix {
#   CONFIG += link_pkgconfig
#   PKGCONFIG += opencv
#}

#INCLUDEPATH += ../Shark/include

#SOURCES += ../Shark/src/Algorithms/*.cpp
