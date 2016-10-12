
CONFIG += console debug_and_release
CONFIG -= app_bundle
QT -= core gui
TEMPLATE = app

CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}

include(ribi.pri)
SOURCES += ribi_main.cpp

unix:!macx{
  # Linux only
  message("Console application, built for Linux")
  message(Host name: $$QMAKE_HOST.name)
  contains(QMAKE_HOST.name,pc-157-92) {
    message("Host is student computer")
    QMAKE_CXX = g++-5
    QMAKE_LINK = g++-5
    QMAKE_CC = gcc-5
    QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++14
  }
  !contains(QMAKE_HOST.name,pc-157-92) {
    message("Host is not student computer")
    QMAKE_CXX = g++-5
    QMAKE_LINK = g++-5
    QMAKE_CC = gcc-5
    QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++14
  }
}


include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

# Boost.Graph and GraphViz, only needed in tests???
LIBS += -lboost_graph
