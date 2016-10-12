#ifndef DISTANCER_H
#define DISTANCER_H

#include "ribi_parameters.h"

namespace ribi {

class simulation
{
public:
  simulation(const parameters& p);
  const auto& get_parameters() const noexcept { return m_parameters; }
  int get_current_generation() const noexcept { return m_current_generation; }

private:
  int m_current_generation;
  parameters m_parameters;
};

///Return something
void do_simulation(const parameters& p);

//' The function that does a simulation.
//' @param max_genetic_distance the maximum number of different loci
//    that two individuals can still produce viable offspring with
//' @param n_generations number of generations
//' @param n_pin number of Phylogeny Inferring Nucleotides
//' @param n_sil number of Species Identity Loci
//' @param pin_mutation_rate PIN mutation rate
//' @param population_size population size
//' @param rgfg_filename the name of the file
//    results (a genotype frequency graph)
//    the results will be written to
//' @param rng_seed random number generator seed
//' @param sampling_interval after how many generations is the population
//    sampled for species abundances
//' @param sil_mutation_rate SIL mutation rate
//' @return nothing. A file with name 'results_genotype_frequency_graph_filename' will be created
//' @export
// [[Rcpp::export]]
void do_simulation_cpp(
  const int max_genetic_distance,
  const int n_generations,
  const int n_pin, //Use int over std::size_t for r
  const int n_sil, //Use int over std::size_t for r
  const double pin_mutation_rate,
  const int population_size,
  const std::string& rgfg_filename, //results_genotype_frequency_graph_filename
  const int rng_seed,
  const int sampling_interval,
  const double sil_mutation_rate
);

} //~namespace ribi

#endif // DISTANCER_H

