USER = $$(USER)
contains(USER, p230198) {
  message(Richels computer)
}

!contains(USER, p230198) {
  message(Not Richels computer)
}


IS_ON_TRAVIS = $$(TRAVIS)

count(IS_ON_TRAVIS, 0) {
  message(Not building on Travis)
}

count(IS_ON_TRAVIS, 1) {
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

!IS_ON_TRAVIS {
  message(Building on local machine)
}


SOURCES += \
  sado_species_test.cpp \
  sado_species_graph_test.cpp
