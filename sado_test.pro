include(any_test_qtcore.pri)
include(sado.pri)
include(sado_test.pri)
include(../gausser/gausser.pri)
include(gsl.pri)
include(phyg.pri)

SOURCES += sado_main_test.cpp

#Boost.Graph
LIBS += -lboost_graph
INCLUDEPATH += ../BoostGraphTutorial/BoostGraphTutorial
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_helper.pri)
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_properties.pri)
