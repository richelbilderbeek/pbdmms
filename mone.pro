include(any_profiling.pri)
include(mone.pri)
include(sado.pri)
include(../gausser/gausser.pri)
include(gsl.pri)

#Boost.Graph
LIBS += -lboost_graph
INCLUDEPATH += ../boost_graph_cookbook_1/boost_graph_cookbook_1
include(../boost_graph_cookbook_1/boost_graph_cookbook_1/boost_graph_cookbook_1_helper.pri)
include(../boost_graph_cookbook_1/boost_graph_cookbook_1/boost_graph_cookbook_1_no_properties.pri)

SOURCES += mone_main.cpp
