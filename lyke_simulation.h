#ifndef LYKE_SIMULATION_H
#define LYKE_SIMULATION_H

#include <fstream>
#include <vector>
#include "lyke_random.h"

struct Individual;

//GLOBALS
extern std::vector<Individual*> population;
extern std::vector<Individual*> nextPopulation; //creates population vectors of individuals
//vector of pointers of type individual
//nullptr: sets the initial state of the individuals of the population at zero.
extern std::ofstream EcoTypeFilestream; //opens excel file
extern std::ofstream HistogramFilestream;//opens excel file
extern std::ofstream DefaultresultsFiles;
//std::ofstream SubstitutionFilestream("substitutions.csv"); //opens excel file

void doStatistics(const std::vector<Individual*>& population); // for calculating average ecotype of the population. Always use pointers when using pointers is considered important

///Calculate the average ecotype of the population. Store in some magic place
void doStatistics(const std::vector<Individual>& population);

void doHistogram(const std::vector<Individual *> &population, int gen); //for making a histogram of the ecotypes. Pointers are your friends, even when not really, but well, you can tolerate them

///Make a histogram of the ecotypes
void doHistogram(const std::vector<Individual>& population, const int gen);

void recreate_defaultresults_output(const std::string& filename);


void iterate(std::vector<Individual*>& population); //Pointers are very important here, as nullptr have an effect on the carrying capacity
void iterate(std::vector<Individual>& population);

void show_output (std::vector<Individual*> population); //Always use pointers if you allow for nullptr values. Ignore const correctness out of wisdom

///Not only display the population. It also, in virtuous secrecy, writes results to a file! HOW COOL IS THAT!
void show_output(const std::vector<Individual>& population) noexcept;


void replace_current_generation_by_new(std::vector<Individual*> population); //Pointers are cool

///Replace 'population' by the global 'next_population'
void replace_current_generation_by_new(std::vector<Individual>& population);

rnd::discrete_distribution calculates_viability(std::vector <Individual*>& population); //Pointers are very vital to use here
rnd::discrete_distribution calculates_viability(const std::vector<Individual>& population);

std::vector<int> create_n_offspring_per_individual(rnd::discrete_distribution& viability);

void viability_selection_on_offspring(std::vector<int>& n_offspring,rnd::discrete_distribution& viability, std::vector<Individual*> population); //It is essential to use copies of pointers here

void viability_selection_on_offspring(std::vector<int>& n_offspring,rnd::discrete_distribution& viability, std::vector<Individual>& population);

#endif // LYKE_SIMULATION_H
