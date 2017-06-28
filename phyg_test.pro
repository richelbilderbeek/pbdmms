include(any_test_qtcore.pri)
include(../gausser/gausser.pri)
include(gsl.pri)
include(jobo.pri)
include(kewe.pri)
include(pbd.pri)
include(phyg.pri)
include(phyg_test.pri)
include(sado.pri)

SOURCES += main_test.cpp

#Boost.Graph
LIBS += -lboost_graph
INCLUDEPATH += ../BoostGraphTutorial/BoostGraphTutorial
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_helper.pri)
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_properties.pri)
