#include "jobo_simulation.h"
#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"
#include <cassert>
#include <iostream>

jobo::simulation::simulation(
  const parameters& parameters
) noexcept
  : m_individuals(parameters.get_population_size()),
    m_parameters{parameters},
    m_rng_engine{42}
{

}

// TODO
// Place get_random_int here, with seed as parameter
// Don't forget to use a different seed for each use of the function

void jobo::simulation::goto_next_generation()
{
  assert(!m_individuals.empty());
  //Get random father
  const auto father = m_individuals.back(); //TODO

  //Get random mother
  const auto mother = m_individuals.front(); //TODO

  //Create kid
  const auto kid = create_offspring(mother, father, m_rng_engine);

  //Overwrite random individual by kid
  m_individuals.back() = kid; //TODO
}
