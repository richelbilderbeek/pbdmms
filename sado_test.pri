
$$(TRAVIS) {
  message(Building on Travis)

  SOURCES += \
   $$PWD/sado_erasure_method_test.cpp \
   $$PWD/sado_next_generation_method_test.cpp \
   $$PWD/sado_parameters_test.cpp \
   $$PWD/sado_results_test.cpp \
   $$PWD/sado_simulation_test.cpp \
   $$PWD/sado_pedigree_test.cpp \
   $$PWD/sado_indiv_test.cpp \
   $$PWD/sado_population_test.cpp
}


SOURCES += \
  sado_species_test.cpp \
  sado_species_graph_test.cpp
