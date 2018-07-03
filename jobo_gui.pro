include(any_gui.pri)
include(jobo.pri)
include(pbd.pri)
include(jobo_gui.pri)
include(jkr.pri)
include(../RibiLibraries/Qwt.pri)
#include(../SurfacePlotter/QtSurfacePlotWidget.pri)

# Boost.Graph
LIBS += -lboost_graph
INCLUDEPATH += ../boost_graph_cookbook_1/boost_graph_cookbook_1
include(../boost_graph_cookbook_1/boost_graph_cookbook_1/boost_graph_cookbook_1_no_graphviz_helper.pri)
include(../boost_graph_cookbook_1/boost_graph_cookbook_1/boost_graph_cookbook_1_no_graphviz_no_properties.pri)

SOURCES += jobo_qtmain.cpp

