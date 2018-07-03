# General setup
include(any_test.pri)

include(gsl.pri)
include(mone.pri)
include(mone_test.pri)
include(pbd.pri)
include(jkr.pri)

# Boost.Graph
LIBS += -lboost_graph
include(../boost_graph_cookbook_1/boost_graph_cookbook_1/boost_graph_tutorial.pri)

include(../RibiUnits/RibiUnits.pri)
