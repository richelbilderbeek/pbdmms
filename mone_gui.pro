# General setup
include(any_gui.pri)

include(gsl.pri)
include(mone.pri)
include(pbd.pri)
include(mone_gui.pri)
include(jkr.pri)
include(../RibiLibraries/Qwt.pri)
include(../SurfacePlotter/QtSurfacePlotWidget.pri)

# Boost.Graph
LIBS += -lboost_graph
include(../boost_graph_cookbook_1/boost_graph_cookbook_1/boost_graph_tutorial.pri)

SOURCES += mone_qtmain.cpp

