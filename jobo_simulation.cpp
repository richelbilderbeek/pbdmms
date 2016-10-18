#include "jobo_simulation.h"
#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"
#include <iostream>

jobo::simulation::simulation(
  const parameters& parameters
) noexcept : m_individuals(parameters.get_population_size()),
    m_parameters{parameters}
{
}

// TODO
// Place get_random_int here, with seed seperate as parameter
// Don't forget to create a different seed for each time you use of the function

