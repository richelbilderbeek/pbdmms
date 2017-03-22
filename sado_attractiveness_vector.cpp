#include "sado_attractiveness_vector.h"

#include "sado_parameters.h"
#include "sado_population.h"

sado::attractiveness_vector sado::get_attractivenesses(
    const population& pop,
    const double preference,
    const double eco_trait,
    const parameters& p)
{
  std::vector<double> as(pop.size(), 0.0);
  int index{0};
  for (auto j = std::cbegin(pop.get_population()); j != std::cend(pop.get_population()); j++)
  {
    const double qj{j->get_q()};
    const double xj{j->get_x()};
    as[index] = p.get_gausser_sm()(preference - qj) * p.get_gausser_se()(eco_trait - xj);
    ++index;
  }
  return as;
}
