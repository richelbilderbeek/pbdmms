CONFIG += console debug_and_release
CONFIG -= app_bundle
QT -= core gui
TEMPLATE = app

CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}

include(ribi.pri)
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

HEADERS += \
    ribi_population_factory.h

SOURCES += \
    ribi_main_test.cpp \
    ribi_dna_test.cpp \
    ribi_helper_test.cpp \
    ribi_individual_test.cpp \
    ribi_parameters_test.cpp \
    ribi_population_test.cpp \
    ribi_results_test.cpp \
    ribi_sil_frequency_phylogeny_test.cpp \
    ribi_sil_frequency_vertex_test.cpp \
    ribi_simulation_test.cpp \
    ribi_species_id_test.cpp \
    ribi_population_factory.cpp

unix:!macx{
  # Linux only
  message("Console application, built for Linux")
  message(Host name: $$QMAKE_HOST.name)
  contains(QMAKE_HOST.name,pc-157-103) {
    message("Host is student computer")
    QMAKE_CXX = g++-4.8
    QMAKE_LINK = g++-4.8
    QMAKE_CC = gcc-4.8
    QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++11
  }
  !contains(QMAKE_HOST.name,pc-157-103) {
    message("Host is not student computer")
    QMAKE_CXX = g++-5
    QMAKE_LINK = g++-5
    QMAKE_CC = gcc-5
    QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++11
  }

  # Boost.Test
  LIBS += -lboost_unit_test_framework

  # gcov
  QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
  LIBS += -lgcov

  # Boost.Graph and GraphViz, only needed in tests
  LIBS += -lboost_graph

}

win32 {
  # Windows
  message(compiling on Windows)
  # Boost.Test
<<<<<<< HEAD
  INCLUDEPATH += C:/local/boost_1_62_0/
=======
  INCLUDEPATH += C:/local/boost_1_62_0
>>>>>>> 1c695b74f5827f7a6959deb2a127c0936b750af3
  LIBS += -LC:/local/boost_1_62_0/lib64-msvc-14.0 -llibboost_unit_test_framework-vc140-mt-gd-1_62
}

