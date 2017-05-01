SOURCES += \
    jaan_main.cpp \
    jaan_parameters.cpp \
    jaan_individual.cpp \
    jaan_simulation.cpp \
    jaan_habitat.cpp

HEADERS += \
    jaan_parameters.h \
    jaan_individual.h \
    jaan_simulation.h \
    jaan_habitat.h


# C++14
CONFIG += c++14
QMAKE_CXX = g++
QMAKE_LINK = g++
QMAKE_CC = gcc
QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++14

# Have a debug and release mode
CONFIG += console debug_and_release
# Profile in release mode
CONFIG(release, debug|release) {

  DEFINES += NDEBUG

  # gprof
  QMAKE_CXXFLAGS += -pg
  QMAKE_LFLAGS += -pg
}
# Measure code coverage and undefined behavior in debug mode
CONFIG(debug, debug|release) {

  # gcov
  QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
  LIBS += -lgcov

  # UBSAN
  QMAKE_CXXFLAGS += -fsanitize=undefined
  QMAKE_LFLAGS += -fsanitize=undefined
  LIBS += -lubsan
}

# No Qt
QT -= core gui
