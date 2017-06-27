include(any_profiling.pri)

include(gsl.pri)
include(jkr.pri)
include(phyg.pri)
include(ribi.pri)
include(pbd.pri)

# Boost.Graph
INCLUDEPATH += ../BoostGraphTutorial/BoostGraphTutorial
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_graphviz_helper.pri)
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_graphviz_no_properties.pri)

SOURCES += ribi_main.cpp
