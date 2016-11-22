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

void doStatistics(const std::vector<Individual*>& population); // for calculating average ecotype of the population
void doHistogram(const std::vector<Individual *> &population, int gen); //for making a histogram of the ecotypes
void recreate_defaultresults_output(const std::string& filename);
void iterate(std::vector <Individual*>& population);
void show_output (std::vector<Individual*> population);
void replace_current_generation_by_new(std::vector<Individual*> population);
rnd::discrete_distribution calculates_viability(std::vector <Individual*>& population);
std::vector<int> create_n_offspring_per_individual(rnd::discrete_distribution& viability);


#endif // LYKE_SIMULATION_H
