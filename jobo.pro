include(any_profiling.pri)
include(jkr.pri)
include(jobo.pri)

# Boost.Graph
LIBS += -lboost_graph
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

SOURCES += $$PWD/jobo_main.cpp
