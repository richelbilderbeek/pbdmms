# General setup
include(any_gui.pri)

include(gsl.pri)
include(ribi.pri)
include(pbd.pri)
include(ribi_gui.pri)
include(jkr.pri)
include(../RibiLibraries/Qwt.pri)
include(../SurfacePlotter/QtSurfacePlotWidget.pri)

# Boost.Graph
LIBS += -lboost_graph
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

SOURCES += ribi_qtmain.cpp

