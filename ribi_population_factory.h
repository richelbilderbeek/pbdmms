#ifndef POPULATION_FACTORY_H
#define POPULATION_FACTORY_H

#include "ribi_population.h"
#include "ribi_individual.h"

namespace ribi {

class population_factory
{
public:

  ///Create a population of n_a indiduals i_a
  population create(
    int n_a, const individual& i_a
  ) const noexcept;

  ///Create a population of n_a indiduals i_a and n_b individuals i_b and n_c individuals i_c
  population create(
    int n_a, const individual& i_a,
    int n_b, const individual& i_b,
    int n_c, const individual& i_c
  ) const noexcept;

  ///Create a population of n_a indiduals i_a and n_b individuals i_b and n_c individuals i_c
  population create(
    int n_a, const individual& i_a,
    int n_b, const individual& i_b,
    int n_c, const individual& i_c,
    int n_d, const individual& i_d,
    int n_e, const individual& i_e
  ) const noexcept;

  population create_population_no_individuals() const noexcept;
  population create_population_one_individual() const noexcept;
  population create_population_two_individuals_one_species() const noexcept;
  population create_population_two_individuals_two_species() const noexcept;
  population create_population_three_individuals_two_species() const noexcept;
  population create_population_three_individuals_one_species_one_possible_species() const noexcept;
};

} //~namespace ribi

#endif // POPULATION_FACTORY_H
