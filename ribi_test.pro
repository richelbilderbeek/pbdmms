# General setup
include(any_test.pri)

include(gsl.pri)
include(jkr.pri)
include(pbd.pri)
include(phyg.pri)
include(ribi.pri)
include(ribi_test.pri)

# Boost.Graph
LIBS += -lboost_graph
INCLUDEPATH += ../BoostGraphTutorial/BoostGraphTutorial
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_graphviz_helper.pri)
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_graphviz_no_properties.pri)

include(../RibiUnits/RibiUnits.pri)
