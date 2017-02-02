include(any_peregrine.pri)
include(pbd.pri)

# Boost.Graph
LIBS += -lboost_graph
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

SOURCES += $$PWD/pbd_main.cpp

OTHER_FILES +=

