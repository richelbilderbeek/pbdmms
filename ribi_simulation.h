#ifndef RIBI_SIMULATION_H
#define RIBI_SIMULATION_H

#include "ribi_parameters.h"
#include "ribi_results.h"
#include "ribi_hopefull_monster.h"

namespace ribi {

class simulation
{
public:
  simulation(const parameters& p);
  void do_one_timestep();
  const auto& get_parameters() const noexcept { return m_parameters; }
  const auto& get_results() const noexcept { return m_results; }
  int get_current_generation() const noexcept { return m_current_generation; }
  void run();


private:
  int m_current_generation;

  ///Kids with parents they cannot mate with
  std::vector<hopefull_monster> m_hopefull_monsters;
  parameters m_parameters;
  population m_population;
  results m_results;
  std::mt19937 m_rng_engine;

  individual create_kid(const std::pair<individual, individual>& parents);
  std::pair<individual, individual> find_parents();
};

///Delete all files created by a simulation run
void clean_simulation(const parameters& p);

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
  const double sil_mutation_rate
);

///Produces a father and a mother that can reproduce from a population.
///The father and mother can be the same individuals, so this function
///can always produce two parents. Use 'find_different_parents' if
///different parents are needed.
///Will throw if population is empty or max_genetic_distance is invalid
/// @param population a population
/// @param max_genetic_distance the number of SILs that two individuals may
///   differ to allow mating
/// @param rng_engine random number generator engine
std::pair<individual, individual> find_parents(
  const population& population,
  const int max_genetic_distance,
  std::mt19937& rng_engine
);

///If there are no potential mates, this function will throw
std::pair<individual, individual> find_different_parents(
  const population& population,
  const int max_genetic_distance,
  std::mt19937& rng_engine
);

///Measure if the kid can mate with at least one parent
///If not, it is considered a hopefull monster
bool kid_is_hopefull_monster(
  const individual& kid,
  const std::pair<individual, individual>& parents,
  const int max_genetic_distance
);

} //~namespace ribi

#endif // RIBI_SIMULATION_H

