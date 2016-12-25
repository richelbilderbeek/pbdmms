include(any_peregrine.pri)
include(jkr.pri)
include(jobo.pri)

# Boost
DEFINES += BOOST_GRAPH_TUTORIAL_NO_GRAPHVIZ
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_graphviz.pri)
LIBS += -lboost_graph

SOURCES += $$PWD/jobo_main.cpp
