include(any_peregrine.pri)
include(jkr.pri)
include(jobo.pri)
include(pbd.pri)

# Boost.Graph
LIBS += -lboost_graph
include(../boost_graph_cookbook_1/boost_graph_cookbook_1/boost_graph_cookbook_1_helper.pri)
include(../boost_graph_cookbook_1/boost_graph_cookbook_1/boost_graph_cookbook_1_no_properties.pri)

SOURCES += $$PWD/jobo_main.cpp
