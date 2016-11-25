CONFIG += console
CONFIG -= app_bundle
QT += core
QT -= gui
TEMPLATE = app

DEFINES += NDEBUG

QMAKE_CXXFLAGS += -Wall -Wextra -Werror -std=c++14

HEADERS += \
    pbd_helper.h \
    lyke_individual.h \
    lyke_random.h \
    lyke_utils.h \
    lyke_simulation.h \
    lyke_parameters.h

SOURCES += \
    pbd_helper.cpp \
    lyke_individual.cpp \
    lyke_random.cpp \
    lyke_simulation.cpp \
    lyke_utils.cpp \
    lyke_main.cpp \
    lyke_parameters.cpp

RESOURCES += \
    lyke.qrc


