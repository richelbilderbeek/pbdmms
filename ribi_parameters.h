#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <string>
#include <utility>

class parameters
{
public:
  parameters(
    const int max_genetic_distance,
    const int n_generations,
    const std::size_t n_pin_loci,
    const std::size_t n_sil_loci,
    const double pin_mutation_rate,
    const int population_size,
    const std::string& results_genotype_frequency_graph_filename,
    const int rng_seed,
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

  ///Chance to have 1 PIN changed in a genome
  double get_pin_mutation_rate() const noexcept { return m_pin_mutation_rate; }

  ///The filename of the file the genotype frequency graph will be written to
  const std::string& get_filename_genotype_frequency_graph() const noexcept { return m_results_genotype_frequency_graph_filename; }

  ///The filename of the file the genotype frequency graph before summarization will be written to
  //std::string get_filename_genotype_frequency_graph_before_summary() const noexcept;
  //std::string get_filename_genotype_frequency_graph_before_summary_as_png() const noexcept;
  //std::string get_filename_genotype_frequency_graph_before_summary_as_svg() const noexcept;

  ///After how many generations is the population sampled for species abundances
  int get_sampling_interval() const noexcept { return m_sampling_interval; }

  ///Chance to have 1 SIL flipped in a genome
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
  int m_rng_seed;

  ///After how many generations is the population sampled for species abundances
  int m_sampling_interval;

  ///Chance to have 1 SIL changed in a genome
  double m_sil_mutation_rate;
};

#endif // PARAMETERS_H
