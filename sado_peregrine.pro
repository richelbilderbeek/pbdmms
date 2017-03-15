include(any_peregrine.pri)

# Qt needed for histogram to png
QT += core gui widgets

include(sado.pri)
include(pbd.pri)
include(histogram_to_png.pri)
include(../gausser/gausser.pri)
include(gsl.pri)

include(../SurfacePlotter/QtSurfacePlotWidget.pri)

#Boost.Graph
LIBS += -lboost_graph
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_helper.pri)
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial_no_properties.pri)

SOURCES += sado_main.cpp
