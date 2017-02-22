include(any_test_qtcore.pri)
include(sado.pri)
include(sado_test.pri)
include(../gausser/gausser.pri)

# New, run 'download_other_githubs' again
include(../Newick/Newick.pri)
include(../RibiLibraries/BigInteger.pri)
include(../RibiClasses/CppFuzzy_equal_to/CppFuzzy_equal_to.pri)

SOURCES += sado_main_test.cpp

#Boost.Graph
LIBS += -lboost_graph
include(../BoostGraphTutorial/BoostGraphTutorial/boost_graph_tutorial.pri)

HEADERS +=
