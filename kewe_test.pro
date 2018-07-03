include(any_test_qtcore.pri)
include(kewe.pri)
include(kewe_test.pri)
include(phyg.pri)

# Boost.Graph
INCLUDEPATH += ../boost_graph_cookbook_1/boost_graph_cookbook_1
include(../boost_graph_cookbook_1/boost_graph_cookbook_1/boost_graph_cookbook_1_no_graphviz_helper.pri)
include(../boost_graph_cookbook_1/boost_graph_cookbook_1/boost_graph_cookbook_1_no_graphviz_no_properties.pri)

include(../gausser/gausser.pri)

SOURCES += kewe_main_test.cpp

