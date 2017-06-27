#ifndef CINE_SIMULATION_H
#define CINE_SIMULATION_H


#include "cine_landscape.h"
#include "cine_population.h"
#include "cine_parameters.h"


#include <vector>		// for vector related commands
#include <functional>
#include <fstream>
#include <cmath>        //Mathematical functions


//Christoph: I left all functions published to be able to access
//them for testing in a different script

///Initialize positions and type of individuals
void ini_positions(population& pop, const int pop_size,
                   const int ncols, const int nrows,
                   const char type, const char smart, const char evolve);

///Grazing of herbivores
void grazing(population& H, landscape& Plots);

///Checks for prey and predators on same field
/// Simulates predation, if successful, prey is removed from pop
void predation_simulation(population& H, population& P, const landscape& patch);

///Create adversary presence clues
void update_adclues(const population& prey, const population& predator, landscape& Plots);


///Creates the inputs for network calculation
std::vector<double> input_info(int delta_x, int delta_y,
                   individual& i,
                   const landscape& my_landscape);

///move based on attractivity values
void smart_movement (std::vector<double>& attractiveness,
                     std::vector<int>& x_movement,
                     std::vector<int>& y_movement,
                     individual& i, const landscape& my_landscape);

///makes use of above funcitons to let an individual move directed by ANN
void ind_movement(individual& i,
                       const landscape& my_landscape,
                       const std::vector<int> layer_nodes);

///Iterate function input_to_movement over entire population
void pop_movement (population& p,
                         const landscape& my_landscape,
                         const std::vector<int> layer_nodes);

///creates vector of collected food over lifetime for a population
std::vector<double> collect_foods(population& xy, const double ANN_cost);

///calculates mean food intake per individual of a population
double calc_total_food(population& xy, const double ANN_cost);

///turns consumed food of individual into proportion of total food consumed --> fitness
std::vector<double> calculate_fitnesses_from_food(population& xy, const double ANN_cost);

///produces new weights in case of mutation
double produce_new_weight(individual& i, int weight_no);

///Mutation in individual
void mutation_i (individual& i, double probability, int mut_type);

///Mutation across population
void mutation_all (population& p, double probability, int mut_type);

///Creation of new generation, parents assigned randomly with probability individual fitness
void new_generation (population& p, std::vector<double> fitv, int popsize);

///create a landscape with n_cols and n_rows
landscape create_landscape(const int n_cols, const int n_rows);

///apply function for plots in landscape
void for_plots(landscape& my_landscape, std::function<void(plot&)> f);


///regenerate grass height
void let_grass_grow(landscape& Plots);

void get_output(population& pop);



void do_simulation(cine_parameters parameter);


#endif // CINE_SIMULATION_H
