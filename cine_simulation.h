#ifndef CINE_SIMULATION_H
#define CINE_SIMULATION_H

#include <vector>
#include "cine_plot.h"
#include "cine_landscape.h"
#include "cine_individual.h"
#include "cine_population.h"

//n_cols: number of columns the patch has
//n_rows: number of rows the patch has

///Checks for prey and predators on same field
/// Simulates predation, if successful, prey is removed from pop
void predation(population& H, population& P, const landscape& patch);

///Move all individuals in a population over a landscape
void movement(population& my_population);

///Move one individual over a landscape
void movement (individual& i, const landscape& my_landscape);

///creates vector of collected food over lifetime for a population
std::vector<double> collect_foods(const population& xy);

///calculates mean food intake per individual of a population
double calc_total_food(const population& xy);

///turns consumed food of individual into proportion of total food consumed --> fitness
std::vector<double> calculate_fitnesses_from_food(const population& xy);


///Creation of new generation, parents assigned randomly with probability individual fitness
void reproduction (population& p, std::vector<double> fitv);

///create a alandscape with n_cols and n_rows
landscape create_landscape(const int n_cols, const int n_rows);


void do_simulation(const int n_cols, const int n_rows);

//void create_ANN();

///Creates a perceptron with size n_input_neurons and n_hidden_neurons and n_output_neurons
///with all weights being initialized to zero
//ANN create_percepton(const int n_input_neurons, const int n_hidden_neurons, const int n_output_neurons);

//Moves the individuals 'xy'
//void movement (int population_size, individual popululation[], int landscape_witdh, int landscape_height);




#endif // CINE_SIMULATION_H
