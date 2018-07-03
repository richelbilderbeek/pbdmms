include(any_gui.pri)
include(../gausser/gausser.pri)
include(jkr.pri)
include(kewe_gui.pri)
include(kewe.pri)
include(phyg.pri)
include(../RibiLibraries/Qwt.pri)

# Boost.Graph
INCLUDEPATH += ../boost_graph_cookbook_1/boost_graph_cookbook_1
include(../boost_graph_cookbook_1/boost_graph_cookbook_1/boost_graph_cookbook_1_no_graphviz_helper.pri)
include(../boost_graph_cookbook_1/boost_graph_cookbook_1/boost_graph_cookbook_1_no_graphviz_no_properties.pri)

SOURCES += kewe_qtmain.cpp

include(../SurfacePlotter/QtSurfacePlotWidget.pri)
