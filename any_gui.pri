# C++14
CONFIG += c++14
QMAKE_CXX = g++-5
QMAKE_LINK = g++-5
QMAKE_CC = gcc-5
# Qt and Qwt do not go well with -Weffc++
# -Wshadow does not go well with Qwt
QMAKE_CXXFLAGS += -std=c++14 -Wall -Wextra -Wnon-virtual-dtor -pedantic -Werror

# Debug and release mode
CONFIG += debug_and_release

# In release mode, define NDEBUG
CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}

# In debug mode, turn on gcov and UBSAN
CONFIG(debug, debug|release) {

  # gcov
  QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
  LIBS += -lgcov

  # UBSAN
  QMAKE_CXXFLAGS += -fsanitize=undefined
  QMAKE_LFLAGS += -fsanitize=undefined
  LIBS += -lubsan
}

# Qt4 and Qt5
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# Prevent Qt for failing with this error:
# qrc_[*].cpp:400:44: error: ‘qInitResources_[*]__init_variable__’ defined but not used
# [*]: the resource filename
QMAKE_CXXFLAGS += -Wno-unused-variable

