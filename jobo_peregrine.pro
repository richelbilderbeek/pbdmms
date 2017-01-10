include(any_peregrine.pri)
include(jkr.pri)
include(jobo.pri)

# Boost
DEFINES += BOOST_GRAPH_TUTORIAL
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)
LIBS += -lboost_graph

SOURCES += $$PWD/jobo_main.cpp
