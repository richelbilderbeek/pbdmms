include(any_test_qtcore.pri)
include(kewe.pri)
include(kewe_test.pri)
include(phyg.pri)

# Boost.Graph
INCLUDEPATH += ../BoostGraphTutorial/BoostGraphTutorial
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_graphviz_helper.pri)
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_graphviz_no_properties.pri)

include(../gausser/gausser.pri)

SOURCES += kewe_main_test.cpp

