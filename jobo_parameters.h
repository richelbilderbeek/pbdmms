#ifndef JOBO_PARAMETERS_H
#define JOBO_PARAMETERS_H

#include "jobo_individuals.h"
#include "jobo_individual.h"
#include <vector>
#include <stdexcept>

using namespace std;
namespace jobo{

class parameters
{
public:
  parameters(
    const int population_size,
    const int seed,
    const double mutation_rate,
    const int duration,
    const int loci
  );

  int get_population_size() const noexcept { return m_population_size;}
  int get_seed() const noexcept {return m_seed;}
  double get_mutation_rate() const noexcept {return m_mutation_rate;}
  int get_duration() const noexcept {return m_duration;}
  int get_n_loci() const noexcept {return m_loci;}

private:
  int m_population_size;
  int m_seed;
  double m_mutation_rate;
  int m_duration;
  int m_loci;
};

bool operator==(const parameters& lhs, const parameters& rhs) noexcept;
bool operator!=(const parameters& lhs, const parameters& rhs) noexcept;

} //~namespace jobo

#endif // JOBO_PARAMETERS_H
