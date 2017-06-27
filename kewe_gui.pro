include(any_gui.pri)
include(../RibiLibraries/Qwt.pri)
include(../gausser/gausser.pri)
include(jkr.pri)
include(kewe.pri)
include(kewe_gui.pri)

INCLUDEPATH += ../BoostGraphTutorial/BoostGraphTutorial
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_graphviz_helper.pri)
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_graphviz_no_properties.pri)

SOURCES += kewe_qtmain.cpp

include(../SurfacePlotter/QtSurfacePlotWidget.pri)
