include(any_profiling.pri)
include(jkr.pri)
include(jobo.pri)
include(pbd.pri)

# Boost.Graph
LIBS += -lboost_graph
INCLUDEPATH += ../boost_graph_cookbook_1/boost_graph_cookbook_1
include(../boost_graph_cookbook_1/boost_graph_cookbook_1/boost_graph_cookbook_1_no_graphviz_helper.pri)
include(../boost_graph_cookbook_1/boost_graph_cookbook_1/boost_graph_cookbook_1_no_graphviz_no_properties.pri)

SOURCES += $$PWD/jobo_main.cpp
