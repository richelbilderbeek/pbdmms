include(any_gui.pri)
include(../RibiLibraries/Qwt.pri)
include(../gausser/gausser.pri)
include(jkr.pri)
include(kewe.pri)
include(kewe_gui.pri)

# Boost.Graph
LIBS += -lboost_graph
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

SOURCES += kewe_qtmain.cpp

include(../SurfacePlotter/QtSurfacePlotWidget.pri)
