# General setup
include(any_gui.pri)

include(gsl.pri)
include(jkr.pri)
include(pbd.pri)
include(phyg.pri)
include(ribi_gui.pri)
include(../RibiLibraries/Qwt.pri)
include(ribi.pri)
include(../SurfacePlotter/QtSurfacePlotWidget.pri)

# Boost.Graph
LIBS += -lboost_graph
INCLUDEPATH += ../BoostGraphTutorial/BoostGraphTutorial
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_graphviz_helper.pri)
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_graphviz_no_properties.pri)

SOURCES += ribi_qtmain.cpp

