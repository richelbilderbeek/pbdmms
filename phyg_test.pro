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
INCLUDEPATH += ../boost_graph_cookbook_1/boost_graph_cookbook_1
include(../boost_graph_cookbook_1/boost_graph_cookbook_1/boost_graph_cookbook_1_helper.pri)
include(../boost_graph_cookbook_1/boost_graph_cookbook_1/boost_graph_cookbook_1_no_properties.pri)
