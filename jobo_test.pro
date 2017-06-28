include(any_test.pri)

# Boost.Graph
LIBS += -lboost_graph
INCLUDEPATH += ../BoostGraphTutorial/BoostGraphTutorial
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_graphviz_helper.pri)
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_graphviz_no_properties.pri)

include(jkr.pri)
include(pbd.pri)
include(pbd_test.pri)
include(jobo.pri)
include(jobo_test.pri)

