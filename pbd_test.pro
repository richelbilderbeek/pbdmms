include(any_test.pri)
include(pbd.pri)
include(pbd_test.pri)

SOURCES += pbd_main_test.cpp

# Boost.Graph
LIBS += -lboost_graph
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)
