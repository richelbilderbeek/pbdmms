include(any_profiling_qtcore.pri)
include(../gausser/gausser.pri)
include(kewe.pri)
include(phyg.pri)

# Boost.Graph
INCLUDEPATH += ../BoostGraphTutorial/BoostGraphTutorial
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_graphviz_helper.pri)
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_graphviz_no_properties.pri)

SOURCES += kewe_main.cpp
