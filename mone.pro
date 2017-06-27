include(any_profiling.pri)

include(gsl.pri)
include(jkr.pri)
include(mone.pri)
include(pbd.pri)

# Boost.Graph
LIBS += -lboost_graph
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

SOURCES += mone_main.cpp
