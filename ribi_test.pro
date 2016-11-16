CONFIG += console debug_and_release
CONFIG -= app_bundle
QT -= core gui
TEMPLATE = app

CONFIG(release, debug|release) {
  DEFINES += NDEBUG
}

include(ribi.pri)
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)
include(../RibiUnits/RibiUnits.pri)

HEADERS += \
    ribi_population_factory.h

SOURCES += \
    ribi_main_test.cpp \
    ribi_create_tally_test.cpp \
    ribi_dna_test.cpp \
    ribi_helper_test.cpp \
    ribi_individual_test.cpp \
    ribi_parameters_test.cpp \
    ribi_population_test.cpp \
    ribi_results_test.cpp \
    ribi_sil_frequency_phylogeny_test.cpp \
    ribi_sil_frequency_vertex_test.cpp \
    ribi_sil_test.cpp \
    ribi_simulation_test.cpp \
    ribi_species_id_test.cpp \
    ribi_population_factory.cpp \
    ribi_sil_frequency_edge_test.cpp \
    ribi_population_factory_test.cpp \
    ribi_menu_dialog_test.cpp \
    ribi_tally_to_str_test.cpp

unix:!macx{
  # Linux only
  message("Console application, built for Linux")
  message(Host name: $$QMAKE_HOST.name)

  CONFIG += c++14
  QMAKE_CXX = g++-5
  QMAKE_LINK = g++-5
  QMAKE_CC = gcc-5
  QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++14

  # gcov
  QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
  LIBS += -lgcov

  # Boost.Test
  LIBS += -lboost_unit_test_framework

  # Boost.Graph and GraphViz, only needed in tests
  LIBS += -lboost_graph
}

win32 {
  # Windows
  message(compiling on Windows)
  # Boost.Test
  INCLUDEPATH += C:/local/boost_1_62_0
  LIBS += -LC:/local/boost_1_62_0/lib64-msvc-14.0 -llibboost_unit_test_framework-vc140-mt-gd-1_62
}

