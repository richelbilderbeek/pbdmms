include(any_peregrine.pri)

# Qt needed for histogram to png
QT += core gui widgets

include(sado.pri)
include(pbd.pri)
include(histogram_to_png.pri)
include(../gausser/gausser.pri)

include(../SurfacePlotter/QtSurfacePlotWidget.pri)

SOURCES += sado_main.cpp
