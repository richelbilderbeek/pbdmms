include(any_gui.pri)
include(jobo.pri)
include(pbd.pri)
include(jobo_gui.pri)
include(jkr.pri)
include(../RibiLibraries/Qwt.pri)
#include(../SurfacePlotter/QtSurfacePlotWidget.pri)

# Boost.Graph
LIBS += -lboost_graph
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_helper.pri)
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_properties.pri)

SOURCES += jobo_qtmain.cpp

