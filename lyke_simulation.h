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

///Calculate the average ecotype of the population. Store in some magic place
void doStatistics(const std::vector<Individual>& population);

///Make a histogram of the ecotypes
void doHistogram(const std::vector<Individual>& population, const int gen, std::ofstream& HistogramFilestream);

void recreate_defaultresults_output(const std::string& filename);

void iterate(std::vector<Individual>& population,std::ofstream& EcoTypeFilestream, std::ofstream& DefaultresultsFiles);

///Not only display the population. It also, in virtuous secrecy, writes results to a file! HOW COOL IS THAT!
void show_output(const std::vector<Individual>& population, std::ofstream& EcoTypeFilestream) noexcept;

///Replace 'population' by the global 'next_population'
void replace_current_generation_by_new(std::vector<Individual>& population);

rnd::discrete_distribution calculates_viability(const std::vector<Individual>& population);

std::vector<int> create_n_offspring_per_individual(rnd::discrete_distribution& viability);

void viability_selection_on_offspring(std::vector<int>& n_offspring,rnd::discrete_distribution& viability, std::vector<Individual>& population);

#endif // LYKE_SIMULATION_H
