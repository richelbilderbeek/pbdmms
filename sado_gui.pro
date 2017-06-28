include(any_gui.pri)
include(../RibiLibraries/Qwt.pri)
include(../gausser/gausser.pri)
include(jkr.pri)
include(sado.pri)
include(gsl.pri)
include(sado_gui.pri)
include(phyg.pri)

SOURCES += sado_qtmain.cpp

include(../SurfacePlotter/QtSurfacePlotWidget.pri)

#Boost.Graph
LIBS += -lboost_graph
INCLUDEPATH += ../BoostGraphTutorial/BoostGraphTutorial
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_helper.pri)
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_properties.pri)
