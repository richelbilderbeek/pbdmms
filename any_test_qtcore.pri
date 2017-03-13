
# Host dependent stuff
message($$QMAKE_HOST.name)
contains(QMAKE_HOST.name, debian) {
  message(Debian detected; use g++)
  QMAKE_CXX = g++
  QMAKE_LINK = g++
  QMAKE_CC = gcc
}

!contains(QMAKE_HOST.name, debian) {
  message(No Debian detected; use g++-5)
  QMAKE_CXX = g++-5
  QMAKE_LINK = g++-5
  QMAKE_CC = gcc-5
}

# C++14

CONFIG += c++14
QMAKE_CXXFLAGS += -std=c++14

# Qt does not go with -Weffc++
# -Wshadow goes bad with BigInteger
QMAKE_CXXFLAGS += -Wall -Wextra -Wnon-virtual-dtor -pedantic -Werror

# Debug and release mode
CONFIG += console debug_and_release

# In release mode, define NDEBUG
CONFIG(release, debug|release) {

  # No assert in release mode
  DEFINES += NDEBUG

  # No Expects and Ensures in release mode
  DEFINES += GSL_UNENFORCED_ON_CONTRACT_VIOLATION

  # gprof
  QMAKE_CXXFLAGS += -pg
  QMAKE_LFLAGS += -pg
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

  # Only in debug mode, Expects and Ensures do check
  DEFINES += GSL_THROW_ON_CONTRACT_VIOLATION
}

# Boost.Test
LIBS += -lboost_unit_test_framework

# Boost.Graph
LIBS += -lboost_graph

# No Qt
QT -= gui

# Fixes
#/usr/include/boost/math/constants/constants.hpp:277: error: unable to find numeric literal operator 'operator""Q'
#   BOOST_DEFINE_MATH_CONSTANT(half, 5.000000000000000000000000000000000000e-01, "5.00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000e-01")
#   ^
QMAKE_CXXFLAGS += -fext-numeric-literals

# Prevent Qt for failing with this error:
# qrc_[*].cpp:400:44: error: ‘qInitResources_[*]__init_variable__’ defined but not used
# [*]: the resource filename
QMAKE_CXXFLAGS += -Wno-unused-variable
