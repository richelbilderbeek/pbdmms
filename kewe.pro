include(any_profiling_qtcore.pri)
include(kewe.pri)

# Boost.Graph
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)
LIBS += -lboost_graph

SOURCES += kewe_main.cpp
