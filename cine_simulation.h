#ifndef CINE_SIMULATION_H
#define CINE_SIMULATION_H

#include <vector>
#include "cine_plot.h"
#include "cine_landscape.h"
#include "cine_individual.h"
#include "cine_population.h"

#include <vector>		// for vector related commands
//#include <numeric>		//needed for accumulate
#include <functional>
#include <fstream>
//#include <math.h>
//#include <string>
#include <opencv/cv.h>
#include <opencv/ml.h>
#include <opencv/highgui.h>
//#include <typeinfo>

//n_cols: number of columns the patch has
//n_rows: number of rows the patch has


///Checks for prey and predators on same field
/// Simulates predation, if successful, prey is removed from pop
void predation_simulation(population& H, population& P, const landscape& patch);


///Sets up the ANN with weights from individual
void setup_ANN(individual& i);


///returns input information for ANN
cv::Mat input_info(int delta_x, int delta_y, individual& i,
                   const landscape& my_landscape, population& adv);

///takes input to ANN and calculates plot attractivity
float ANN_calculation(cv::Mat& inputs);

///Normalize attractiveness values
void calc_relative_attractiveness (std::vector<float>& attractiveness);

///move based on attractivity values
void smart_movement (std::vector<float>& attractiveness,
                     std::vector<int>& x_movement,
                     std::vector<int>& y_movement,
                     individual& i, landscape my_landscape);



///Move all individuals in a population over a landscape
void random_movement(population& my_population);

///makes use of above funcitons to let an individual move directed by ANN
void input_to_movement(individual& i, const landscape& my_landscape, population& adv);

///Iterate function input_to_movement over entire population
void smart_pop_movement (population& p, const landscape& my_landscape, const population& adv);

///Move one individual over a landscape
void random_movement (individual& i, const landscape& my_landscape);

///creates vector of collected food over lifetime for a population
std::vector<double> collect_foods(const population& xy);

///calculates mean food intake per individual of a population
double calc_total_food(const population& xy);

///turns consumed food of individual into proportion of total food consumed --> fitness
std::vector<double> calculate_fitnesses_from_food(const population& xy);

///produces new weights in case of mutation
float produce_new_weight(individual& i, int weight_no);

///Mutation in individual
void mutation_i (individual& i, float probability);

///Mutation across population
void mutation_all (population& p, float probability);

///Creation of new generation, parents assigned randomly with probability individual fitness
void new_generation (population& p, std::vector<double> fitv);

///create a landscape with n_cols and n_rows
landscape create_landscape(const int n_cols, const int n_rows);

///apply function for plots in landscape
void for_each(landscape& my_landscape, std::function<void(plot&)> f);


///regenerate grass height
void let_grass_grow(landscape& Plots);


void do_simulation(const int n_cols, const int n_rows);

void create_ANN();

///Creates a perceptron with size n_input_neurons and n_hidden_neurons and n_output_neurons
///with all weights being initialized to zero
//ANN create_percepton(const int n_input_neurons,
//const int n_hidden_neurons, const int n_output_neurons);

//Moves the individuals 'xy'
//void movement (int population_size, individual popululation[],
//int landscape_witdh, int landscape_height);




#endif // CINE_SIMULATION_H
