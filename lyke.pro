CONFIG += console debug_and_release
CONFIG -= app_bundle
QT += core
QT -= gui
TEMPLATE = app

CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}

QMAKE_CXX = g++-5
QMAKE_LINK = g++-5
QMAKE_CC = gcc-5
# -Weffc++ does not play well with Qt
QMAKE_CXXFLAGS += -Wall -Wextra -Werror -std=c++14

# gcov
QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
LIBS += -lgcov

HEADERS += \
    lyke_individual.h \
    lyke_random.h \
    lyke_utils.h

SOURCES += \
    lyke_individual.cpp \
    lyke_random.cpp \
    lyke_simulation.cpp \
    lyke_utils.cpp \
    lyke_main.cpp

RESOURCES += \
    lyke.qrc


