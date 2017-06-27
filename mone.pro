include(any_profiling.pri)
include(mone.pri)
include(sado.pri)
include(../gausser/gausser.pri)
include(gsl.pri)

#Boost.Graph
LIBS += -lboost_graph
INCLUDEPATH += ../BoostGraphTutorial/BoostGraphTutorial
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_helper.pri)
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_properties.pri)

SOURCES += mone_main.cpp
