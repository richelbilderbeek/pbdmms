include(any_profiling_qtcore.pri)
include(../gausser/gausser.pri)
include(kewe.pri)
include(phyg.pri)

# Boost.Graph
INCLUDEPATH += ../boost_graph_cookbook_1/boost_graph_cookbook_1
include(../boost_graph_cookbook_1/boost_graph_cookbook_1/boost_graph_cookbook_1_no_graphviz_helper.pri)
include(../boost_graph_cookbook_1/boost_graph_cookbook_1/boost_graph_cookbook_1_no_graphviz_no_properties.pri)

SOURCES += kewe_main.cpp
