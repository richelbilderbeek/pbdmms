include(any_test.pri)
include(pbd.pri)
include(pbd_test.pri)

SOURCES += pbd_main_test.cpp

# Boost.Graph and GraphViz, only needed in tests
LIBS += -lboost_graph
