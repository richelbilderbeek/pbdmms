include(any_test.pri)
include(pbd.pri)
include(pbd_test.pri)

# Boost.Graph and GraphViz, only needed in tests
LIBS += -lboost_graph
