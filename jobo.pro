include(any_profiling.pri)
include(jkr.pri)
include(jobo.pri)
include(pbd.pri)

# Boost.Graph
LIBS += -lboost_graph
INCLUDEPATH += ../BoostGraphTutorial/BoostGraphTutorial
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_graphviz_helper.pri)
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_graphviz_no_properties.pri)

SOURCES += $$PWD/jobo_main.cpp
