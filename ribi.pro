include(any_profiling.pri)

include(gsl.pri)
include(jkr.pri)
include(phyg.pri)
include(ribi.pri)
include(pbd.pri)

# Boost.Graph
INCLUDEPATH += ../boost_graph_cookbook_1/boost_graph_cookbook_1
include(../boost_graph_cookbook_1/boost_graph_cookbook_1/boost_graph_cookbook_1_no_graphviz_helper.pri)
include(../boost_graph_cookbook_1/boost_graph_cookbook_1/boost_graph_cookbook_1_no_graphviz_no_properties.pri)

SOURCES += ribi_main.cpp
