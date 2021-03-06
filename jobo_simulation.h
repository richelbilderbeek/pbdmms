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
  void do_timestep();
  parameters get_parameters() const noexcept { return m_parameters;}
  individuals get_individuals() const noexcept { return m_individuals;}

  const results& get_results() const noexcept { return m_results; }
        results& get_results()       noexcept { return m_results; }

  void run();

  void set_individuals(const individuals& is);

private:
  ///Individuals of current generations
  individuals m_individuals;
  const parameters m_parameters;
  mt19937 m_rng_engine;
  results m_results;
};

vector<int> get_random_ints(mt19937& rng_engine, const int &n);
vector<double> get_random_doubles(mt19937& rng_engine, const int &n);
vector<int> get_random_parents(mt19937& rng_engine, int population_size);
int get_random_parent(mt19937& rng_engine, const int &population_size);

///Create the next generation
///Will have the size of the current population
individuals create_next_generation(
    const individuals& population,
    const parameters& parameters,
    mt19937 &rng_engine
);

//double calc_competition(const std::vector<individual> &individuals, const int &i);
//double get_genetic_fitness(const individual &i);
//double calc_survivability(
//    const double &fitness_gen,
//    const double &comp,
//    const int &population_size
//);
//double gauss(int capitals_in_genotype, int max_capitals);
//int count_capitals (const std::string genotype);
//int count_lowercase (const std::string genotype);

void set_individuals(simulation& s, vector<individual> next_population);
individuals create_initial_population(const parameters& parameters);
vector<genotype> get_unique_genotypes(const std::vector<individual>& individuals);

int number_species(vector<individual> individuals);
double calc_chance_dead_kids(const genotype &w, const genotype &q);
int get_n_unviable_species(const std::vector<genotype> &vector_of_genotypes);

///Creates the four possible genotypes, ab, aB, Ab and AB (aB is inviable)
std::vector<genotype> create_test_genotypes_1();

std::vector<genotype> collect_viable_genotypes(const std::vector<individual>& individuals);
std::vector<individual> remove_inviable_species(const std::vector<individual>& individuals);

int count_good_species(const std::vector<genotype> &viable_population);
int count_possible_species(const std::vector<individual> &individuals);

} //~namespace jobo

#endif // JOBO_SIMULATION_H
