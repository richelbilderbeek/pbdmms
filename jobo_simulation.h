#ifndef JOBO_SIMULATION_H
#define JOBO_SIMULATION_H

#include <set>
#include <random>
#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"
#include "jobo_results.h"
#include <stdexcept>

using namespace std;
namespace jobo {

class simulation
{
public:
  simulation(const parameters& parameters) noexcept;
  parameters get_parameters() const noexcept { return m_parameters;}
  individuals get_individuals() const noexcept { return m_individuals;}

private:
  ///Individuals of current generations
  individuals m_individuals;
  const parameters m_parameters;
  mt19937 m_rng_engine;
  results m_results;
};

vector<int> get_random_ints(mt19937& rng_engine, int n);
vector<double> get_random_doubles(mt19937& rng_engine, int n);
vector<int> get_random_parents(mt19937& rng_engine, int population_size);

  ///Go the next generations
vector<individual> goto_next_generation(
  vector<individual> individuals,
  const double mutation_rate,
  mt19937& rng_engine
);

vector<genotype> get_unique_genotypes(std::vector<individual> individuals);
vector<individual> extinction_low_fitness(vector<individual> new_individuals);
vector<individual> connect_generations(vector<individual>
individuals, const double mutation_rate, mt19937 &rng_engine);
int number_species(vector<individual> individuals);
double calc_chance_dead_kids(genotype w, genotype q);
vector<double> get_chances_dead_kids(vector<genotype> vector_of_genotypes);
int get_n_unviable_species(std::vector<genotype> vector_of_genotypes);
vector<genotype> create_test_population_1(int time);
int count_good_species(std::vector<individual> individuals);
int count_possible_species(std::vector<individual> individuals);
void do_timestep(const parameters& parameters, int generations);

} //~namespace jobo

#endif // JOBO_SIMULATION_H
