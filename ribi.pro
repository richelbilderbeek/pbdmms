include(any_profiling.pri)
include(jkr.pri)
include(ribi.pri)
include(pbd.pri)

# Boost.Graph
LIBS += -lboost_graph
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

SOURCES += ribi_main.cpp
