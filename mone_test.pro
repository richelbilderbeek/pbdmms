# General setup
include(any_test.pri)

include(gsl.pri)
include(mone.pri)
include(mone_test.pri)
include(pbd.pri)
include(jkr.pri)

# Boost.Graph
LIBS += -lboost_graph
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

include(../RibiUnits/RibiUnits.pri)
