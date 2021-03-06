#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <iosfwd>
#include <string>
#include <utility>
#include "ribi_probability.h"

namespace ribi {

class parameters
{
public:
  explicit parameters(
    const int max_genetic_distance,
    const int n_generations,
    const std::size_t n_pin_loci,
    const std::size_t n_sil_loci,
    const probability pin_mutation_rate,
    const int population_size,
    const std::string& rgfgraph_filename, //results_genotype_frequency_graph_filename
    const unsigned int rng_seed,
    const probability sil_mutation_rate
  );

  ///Number of PIN per individual (PIN: Phylogeny Inferring Nucleotides)
  std::size_t get_n_pin_loci() const noexcept { return m_n_pin_loci; }

  ///Number of SIL per individual (SIL: Species Identity Loci)S
  std::size_t get_n_sil_loci() const noexcept { return m_n_sil_loci; }

  ///RNG seed
  int get_rng_seed() const noexcept { return m_rng_seed; }

  ///Number of generations the simulation will run
  int get_n_generations() const noexcept { return m_n_generations; }

  ///The constant population size
  int get_population_size() const noexcept { return m_population_size; }

  ///The maximum number of alleles two individuals may differ
  ///and still produce viable offspring
  int get_max_genetic_distance() const noexcept { return m_max_genetic_distance; }

  ///Per-locus probabilty of a mutation in the PIN
  auto get_pin_mutation_rate() const noexcept { return m_pin_mutation_rate; }

  ///The filename of the file the genotype frequency graph will be written to
  const std::string& get_filename_genotype_frequency_graph() const noexcept
  {
    return m_results_genotype_frequency_graph_filename;
  }

  std::string get_nltt_plot_filename() const noexcept { return "ribi_nltt_plot.csv"; }
  std::string get_ltt_plot_filename() const noexcept { return "ribi_ltt_plot.csv"; }

  ///Per-locus probabilty of a mutation in the SIL
  auto get_sil_mutation_rate() const noexcept { return m_sil_mutation_rate; }

private:

  ///The maximum number of alleles two individuals may differ
  ///and still produce viable offspring
  int m_max_genetic_distance;

  ///Number of generations the simulation will run
  int m_n_generations;

  ///Number of PINs per individual (PIN: Phylogeny Inferring Nucleotides)
  std::size_t m_n_pin_loci;

  ///Number of SILs per individual (SIL: Species Identity Loci)
  std::size_t m_n_sil_loci;

  ///Chance to have 1 PIN changed in a genome
  probability m_pin_mutation_rate;

  ///The constant population size
  int m_population_size;

  ///The filename of the file the results will be written to
  std::string m_results_genotype_frequency_graph_filename;

  ///RNG seed
  unsigned int m_rng_seed;

  ///Chance to have 1 SIL changed in a genome
  probability m_sil_mutation_rate;

  friend std::ostream& operator<<(std::ostream& os, const parameters& p);
  friend std::istream& operator>>(std::istream& is, parameters& p);
};

bool operator==(const parameters& lhs, const parameters& rhs) noexcept;
bool operator!=(const parameters& lhs, const parameters& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const parameters& p);
std::istream& operator>>(std::istream& is, parameters& p);

///Create parameters used in profiling
parameters create_profiling_parameters() noexcept;

///Create (close to) as-simple-as-possible testing parameters
parameters create_test_parameters_1() noexcept;

///Create testing parameters that show a nice example
parameters create_test_parameters_2() noexcept;

///Create testing parameters that caused a crash
parameters create_test_parameters_3() noexcept;

///Extract the filename to which the LTT plot gets saved to
std::string get_ltt_plot_filename(const parameters& p) noexcept;

///Extract the filename to which the nLTT plot gets saved to
std::string get_nltt_plot_filename(const parameters& p) noexcept;

///Extract the number of generations in parameters
int get_n_generations(const parameters& p) noexcept;

///Extract the RNG seed from the parameters
int get_rng_seed(const parameters& p) noexcept;

///Will throw if filename is absent or parameters are invalid
parameters load_parameters(const std::string& filename);

///Save the parameters to a file
void save_parameters(const parameters& p, const std::string& filename);


} //~namespace ribi

#endif // PARAMETERS_H
