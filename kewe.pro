
CONFIG += console debug_and_release
CONFIG -= app_bundle
QT -= core gui
TEMPLATE = app

CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}

HEADERS += \
    kewe_random.h

SOURCES += \
    kewe_adapspc4.cpp

unix:!macx{
  # Linux only
  message("Console application, built for Linux")
  message(Host name: $$QMAKE_HOST.name)
  contains(QMAKE_HOST.name,pc-157-92) {
    message("Host is student computer")
    QMAKE_CXX = g++-4.8
    QMAKE_LINK = g++-4.8
    QMAKE_CC = gcc-4.8
    QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -std=c++1y
    # Sloppy, to have a successful Travis build
    # -Werror
  }
  !contains(QMAKE_HOST.name,pc-157-92) {
    message("Host is not student computer")
    QMAKE_CXX = g++-5
    QMAKE_LINK = g++-5
    QMAKE_CC = gcc-5
    QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -std=c++14
    # Sloppy, to have a successful Travis build
    # -Werror
  }
}

