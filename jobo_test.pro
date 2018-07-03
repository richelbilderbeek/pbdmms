include(any_test.pri)

# Boost.Graph
LIBS += -lboost_graph
INCLUDEPATH += ../boost_graph_cookbook_1/boost_graph_cookbook_1
include(../boost_graph_cookbook_1/boost_graph_cookbook_1/boost_graph_cookbook_1_no_graphviz_helper.pri)
include(../boost_graph_cookbook_1/boost_graph_cookbook_1/boost_graph_cookbook_1_no_graphviz_no_properties.pri)

include(gsl.pri)
include(jkr.pri)
include(pbd.pri)
include(pbd_test.pri)
include(jobo.pri)
include(jobo_test.pri)

