#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <iosfwd>
#include <string>
#include <utility>

namespace ribi {

class parameters
{
public:
  explicit parameters(
    const int max_genetic_distance,
    const int n_generations,
    const std::size_t n_pin_loci,
    const std::size_t n_sil_loci,
    const double pin_mutation_rate,
    const int population_size,
    const std::string& rgfgraph_filename, //results_genotype_frequency_graph_filename
    const unsigned int rng_seed,
    const int sampling_interval,
    const double sil_mutation_rate
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
  double get_pin_mutation_rate() const noexcept { return m_pin_mutation_rate; }

  ///The filename of the file the genotype frequency graph will be written to
  const std::string& get_filename_genotype_frequency_graph() const noexcept
  {
    return m_results_genotype_frequency_graph_filename;
  }

  ///The filename of the file the genotype frequency graph before summarization will be written to
  //std::string get_filename_genotype_frequency_graph_before_summary() const noexcept;
  //std::string get_filename_genotype_frequency_graph_before_summary_as_png() const noexcept;
  //std::string get_filename_genotype_frequency_graph_before_summary_as_svg() const noexcept;

  ///After how many generations is the population sampled for species abundances
  int get_sampling_interval() const noexcept { return m_sampling_interval; }

  ///Per-locus probabilty of a mutation in the SIL
  double get_sil_mutation_rate() const noexcept { return m_sil_mutation_rate; }

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
  double m_pin_mutation_rate;

  ///The constant population size
  int m_population_size;

  ///The filename of the file the results will be written to
  std::string m_results_genotype_frequency_graph_filename;

  ///RNG seed
  unsigned int m_rng_seed;

  ///After how many generations is the population sampled for species abundances
  int m_sampling_interval;

  ///Chance to have 1 SIL changed in a genome
  double m_sil_mutation_rate;

  friend std::ostream& operator<<(std::ostream& os, const parameters& p);
  friend std::istream& operator>>(std::istream& is, parameters& p);
};

bool operator==(const parameters& lhs, const parameters& rhs) noexcept;
bool operator!=(const parameters& lhs, const parameters& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const parameters& p);
std::istream& operator>>(std::istream& is, parameters& p);

///Create (close to) as-simple-as-possible testing parameters
parameters create_test_parameters_1() noexcept;

///Create testing parameters that show a nice example
parameters create_test_parameters_2() noexcept;

///Will throw if filename is absent or parameters are invalid
parameters load_parameters(const std::string& filename);

///Save the parameters to a file
void save_parameters(const parameters& p, const std::string& filename);


} //~namespace ribi

#endif // PARAMETERS_H
