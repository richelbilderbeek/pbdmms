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
    main_test.cpp \
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


# Must use g++-4.8 and C++0x due to compatibility with R
#QMAKE_CXX = g++-4.8
#QMAKE_LINK = g++-4.8
#QMAKE_CC = gcc-4.8
#QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++11
#SOURCES += /usr/include/boost/test/impl/unit_test_suite.ipp
#HEADERS += /usr/include/boost/test/impl/unit_test_suite.ipp

# Testing facilities need no R, but the shared code does
# Use g++5 to find boost::unit_test::ut_detail::normalize_test_case_name (will give segmentation fault with g++-4.8)
QMAKE_CXX = g++-5
QMAKE_LINK = g++-5
QMAKE_CC = gcc-5
QMAKE_CXXFLAGS += -Wall -Wextra -Weffc++ -Werror -std=c++11

# Boost.Test
LIBS += -lboost_unit_test_framework

# gcov
QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
LIBS += -lgcov

# Boost.Graph and GraphViz, only needed in tests
LIBS += -lboost_graph
