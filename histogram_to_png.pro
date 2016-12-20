HEADERS += \
    pbd_helper.h \
    histogram_to_png.h

SOURCES += \
    pbd_helper.cpp \
    histogram_to_png.cpp \
    histogram_to_png_main.cpp

include(../SurfacePlotter/QtSurfacePlotWidget.pri)
include(any_gui.pri)
