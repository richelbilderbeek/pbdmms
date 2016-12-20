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
    const int n_generations,
    const int n_loci,
    const double fitness_threshold
  );

  int get_population_size() const noexcept { return m_population_size;}
  int get_seed() const noexcept {return m_seed;}
  double get_mutation_rate() const noexcept {return m_mutation_rate;}
  int get_generations() const noexcept {return m_n_generations;}
  int get_n_loci() const noexcept {return m_loci;}
  double get_fitness_threshold() const noexcept {return m_fitness_threshold;}
  friend std::ostream& operator<<(std::ostream& os, const parameters& p);
  friend std::istream& operator>>(std::istream& is, parameters& p);

private:
  int m_population_size;
  int m_seed;
  double m_mutation_rate;
  int m_n_generations;
  int m_loci;
  double m_fitness_threshold;
};

parameters create_test_parameters_1() noexcept;

parameters load_parameters(const std::string& filename);
void save_parameters(const parameters& p, const std::string& filename);
std::ostream& operator<<(std::ostream& os, const parameters& p);
std::istream& operator>>(std::istream& is, parameters& p);

bool operator==(const parameters& lhs, const parameters& rhs) noexcept;
bool operator!=(const parameters& lhs, const parameters& rhs) noexcept;

} //~namespace jobo

#endif // JOBO_PARAMETERS_H
