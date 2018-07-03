include(any_test.pri)
include(pbd.pri)
include(pbd_test.pri)

SOURCES += pbd_main_test.cpp

# Boost.Graph
LIBS += -lboost_graph
include(../boost_graph_cookbook_1/boost_graph_cookbook_1/boost_graph_tutorial.pri)
