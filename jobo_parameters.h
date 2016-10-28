#ifndef JOBO_PARAMETERS_H
#define JOBO_PARAMETERS_H

namespace jobo {

class parameters
{
public:
  parameters(const int n_loci,
    const int population_size,
    const int seed,
    const double mutation_rate,
    const int generations
  );

  int get_n_loci() const noexcept { return m_n_loci; }
  int get_population_size() const noexcept { return m_population_size;}
  int get_seed() const noexcept {return m_seed;}
  double get_mutation_rate() const noexcept {return m_mutation_rate;}
  int get_generations() const noexcept {return m_generations;}

private:
  int m_n_loci;
  int m_population_size;
  int m_seed;
  double m_mutation_rate;
  int m_generations;
  };

bool operator==(const parameters& lhs, const parameters& rhs) noexcept;
bool operator!=(const parameters& lhs, const parameters& rhs) noexcept;

} //~namespace jobo

#endif // JOBO_PARAMETERS_H
