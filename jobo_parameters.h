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
  parameters(const int population_size,
    const int seed,
    const double mutation_rate,
    const int n_generations,
    const int n_loci,
    const std::string& ltt_plot_filename_vi = "jobo_ltt_viables.csv",
    const std::string& ltt_plot_filename_in = "jobo_ltt_inviables.csv",
    const std::string& nltt_plot_filename_v = "jobo_nltt_viables.csv",
    const std::string& nltt_plot_filename_i = "jobo_nltt_inviables.csv");

  int get_population_size() const noexcept { return m_population_size;}
  int get_seed() const noexcept {return m_seed;}
  double get_mutation_rate() const noexcept {return m_mutation_rate;}
  int get_generations() const noexcept {return m_n_generations;}
  const std::string& get_ltt_plot_filename_vi() const noexcept { return m_ltt_plot_filename_vi;}
  const std::string& get_ltt_plot_filename_in() const noexcept { return m_ltt_plot_filename_in;}
  const std::string& get_nltt_plot_filename_v() const noexcept { return m_nltt_plot_filename_v;}
  const std::string& get_nltt_plot_filename_i() const noexcept { return m_nltt_plot_filename_i;}
  int get_n_loci() const noexcept {return m_loci;}

private:
  int m_population_size;
  int m_seed;
  double m_mutation_rate;
  int m_n_generations;
  int m_loci;
  std::string m_ltt_plot_filename_vi;
  std::string m_ltt_plot_filename_in;
  std::string m_nltt_plot_filename_v;
  std::string m_nltt_plot_filename_i;

  friend std::ostream& operator<<(std::ostream& os, const parameters& p);
  friend std::istream& operator>>(std::istream& is, parameters& p);
};

////Close to simplest parameter set
parameters create_test_parameters_1() noexcept;

///Parameters used for profiling
parameters create_profiling_parameters() noexcept;

parameters load_parameters(const std::string& filename);
void save_parameters(const parameters& p, const std::string& filename);
std::ostream& operator<<(std::ostream& os, const parameters& p);
std::istream& operator>>(std::istream& is, parameters& p);

bool operator==(const parameters& lhs, const parameters& rhs) noexcept;
bool operator!=(const parameters& lhs, const parameters& rhs) noexcept;

} //~namespace jobo

#endif // JOBO_PARAMETERS_H
