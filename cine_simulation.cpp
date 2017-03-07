#include "cine_simulation.h"
#include "cine_parameters.h"
#include "cine_plot.h"
#include "cine_landscape.h"
#include "cine_individual.h"
#include "cine_population.h"
#include "cine_ann.h"
#include <iostream>
#include <cassert>
#include <random>		//random number generation
#include <vector>		// for vector related commands
#include <numeric>		//needed for accumulate
#include <functional>
#include <fstream>
#include <math.h>
#include <string>
#include <cmath>        //Mathematical functions
#include <algorithm>    // for shuffle function
#include <chrono>
//#include "cine_simulation.hpp"


using namespace std;
//using namespace simulation_detail;

//put distributions to local classes
//Hanno: global state!

///Initialise random number generator
std::mt19937 rng(chrono::high_resolution_clock::now().time_since_epoch().count());
//std::uniform_real_distribution<double> dist1(0.0, 1.0);	// generate dist 0-1, pred. risk on patch
//std::uniform_int_distribution<> dist2(0, 9);        // generate dist 0-9, init pos of ind.
//std::uniform_int_distribution<> dist3(-1, 1);       // generate dist -1/1: Movement
//std::uniform_real_distribution<double> dist4(0.0, 1.0); assign of offspring, combine with dist1?
//std::uniform_real_distribution<double> dist5(0.0, 1.0); // movement, combine with dist1?
//std::uniform_real_distribution<double> dist6(0.0, 1.0); // weight mutation



///Functions///

///initialize individual positions and type

void ini_positions(population& pop, const int pop_size,
                   const int ncols, const int nrows,
                   const char type, const char smart, const char evolve){
    //assign positions to prey&predator
    // distribution of random positions 0-9
    std::uniform_int_distribution<> dist1(0, (ncols - 1));
    std::uniform_int_distribution<> dist2(0, (nrows - 1));

    for (int j = 0; j < pop_size; ++j) {
        pop[j].setPosition(dist1(rng), dist2(rng));
        pop[j].type(type);
        pop[j].smart(smart);
        pop[j].evolve(evolve);
    }
}

///Function for grazing executed by herbivores
/// so far not corrected for individuals on same plot, necessary?
/// --> might dilute positive effects of network control
void grazing(population& H, landscape& Plots){
    for (int l = 0; l < static_cast<int>(H.size()); ++ l) {
        // Attention: correct for two individuals on same plot
        //prey takes up food from currently occupied plot
        H[l].food_update(Plots(H[l].xposition(), H[l].yposition()).grass_height());
        //consumed grass is depleted from plot
        Plots(H[l].xposition(),H[l].yposition()).grass_consumption();
    }
}


///simulates predation. If predator and prey occupy same patch,
///predation is successfull with probability m_Risk.
/// Predation results in deletion of prey individual & food_update by predator
void predation_simulation(population& H, population& P, const landscape& patch){

    shuffle(P.begin(), P.end(), rng);  // randomize order of predators, ToDo TEST

    for (int m = 0; m < static_cast<int>(P.size()); ++m) { // loop over predator individuals
        int h = 0;
        while (h < static_cast<int>(H.size()))
        {
            bool killed =false;
            if ((H[h].xposition() == P[m].xposition()) &&
                    (H[h].yposition() == P[m].yposition()))
            {
                bernoulli_distribution
                        bernoulli_d(patch(P[m].xposition(), P[m].yposition()).returnRisk());
                if (bernoulli_d(rng) == 1) {
                    P[m].food_update(1.0);
                    H[h] = H.back();
                    H.pop_back();   // Order of individuals changed
                    killed = true;
                }
            }
            if (!killed) ++h;
        }
    }
}

/////Applies a function to all elements of the landscape
void for_plots(landscape& my_landscape, std::function<void(plot&)> f)
{
    for (int i = 0; i < static_cast<int>(my_landscape.xsize()); ++i)
    {
        for (int j = 0; j < static_cast<int>(my_landscape.ysize()); ++j)
        {
            assert(i >= 0);
            assert(i < static_cast<int>(my_landscape.xsize()));
            assert(j >= 0);
            assert(j < static_cast<int>(my_landscape.ysize()));
            f(my_landscape(i, j));
        }
    }
}


///To bring adversary presence clues up to date
/// Converges to 3!!!
void update_adclues(const population& prey, const population& predator, landscape& Plots){
    //previously produced clues decay
    for_plots(Plots, [](plot& p) { p.set_preyclues(p.return_preyclues() * 0.75); } );
    //New clues are produced
    for (int i = 0; i < static_cast<int>(prey.size()); ++i){
        plot X = Plots(prey[i].xposition(), prey[i].yposition());
        X.set_preyclues(X.return_preyclues() + 1.0);
        Plots(prey[i].xposition(), prey[i].yposition()) = X;

    }
    //Same for predator
    for_plots(Plots, [](plot& p) { p.set_predclues(p.return_predclues() * 0.75); } );

    for (int i = 0; i < static_cast<int>(predator.size()); ++i){
        plot X = Plots(predator[i].xposition(), predator[i].yposition());
        X.set_predclues(X.return_predclues() + 1.0);
        Plots(predator[i].xposition(), predator[i].yposition()) = X;

    }
}



///returns input information for ANN
vector<double> input_info(int delta_x, int delta_y,
                          individual& i,
                          const landscape& my_landscape){

    const int sz{static_cast<int>(my_landscape.xsize())};
    const int sy{static_cast<int>(my_landscape.ysize())};


    int pos_x = (i.xposition() + delta_x + sz) % sz;
    int pos_y = (i.yposition() + delta_y + sy) % sy;


    plot patch1 = my_landscape(pos_x, pos_y);

    vector<double> inputs(3);

    uniform_real_distribution<double> dist1(-0.25, 0.25);
    uniform_real_distribution<double> dist2(-0.1, 0.1);
    uniform_real_distribution<double> dist3(-0.2, 0.2);


    inputs[0] = (patch1.grass_height() + dist1(rng));
    inputs[1] = (patch1.returnRisk() + dist2(rng));
    if (i.type() == 'p' ){
        inputs[2] = (patch1.return_preyclues() + dist3(rng));
    }
    else if (i.type() == 'h'){
        inputs[2] = (patch1.return_predclues() + dist3(rng));
    }
    //    to activate, give adv as function argument
    //    double adv_count = 0.0;
    //    for (int m = 0; m < static_cast<int>(adv.size()); ++m){
    //        if(adv[m].xposition() == patch1.xposition() && adv[m].yposition() == patch1.yposition())
    //            adv_count += 1.00;
    //    }

    //    inputs[2] = adv_count;
    //
    return inputs;
}

///move based on attractivity values
void smart_movement (std::vector<double>& attractiveness,
                     std::vector<int>& x_movement,
                     std::vector<int>& y_movement,
                     individual& i, const landscape& my_landscape){

    const int sz{static_cast<int>(my_landscape.xsize())};
    const int sy{static_cast<int>(my_landscape.ysize())};
    assert(sz != 0 && sy != 0);

    //        To choose fields with probabilities based on attractiveness values
    //    // smart movement distribution
    //    std::uniform_real_distribution<double> dist(0.0, 1.0);

    //    double r2 = dist(rng);
    //    double prob = 0;

    //    for (int j = 0; j < static_cast<int>(attractiveness.size()); ++j) {

    //        prob += attractiveness[j];

    //        if (r2 <= prob) {
    //            i.setPosition(
    //                        (i.xposition() + x_movement[j] + sz) % sz,
    //                        (i.yposition() + y_movement[j] + sy) % sy
    //                        );
    //            break; //Does it break the loop?
    //        }
    //    }
    //
    // choosing the field with highest attractiveness
    double single_attr = attractiveness[0];
    int highest_index = 0;
    for (int j = 0; j < static_cast<int>(attractiveness.size()); ++j) {
        if (attractiveness[j] > single_attr){
            highest_index = j;
            single_attr = attractiveness[j];
        }
    }

    i.setPosition(
                (i.xposition() + x_movement[highest_index] + sz) % sz,
                (i.yposition() + y_movement[highest_index] + sy) % sy
                );
}


///makes use of above funcitons to let an individual move directed by ANN
void ind_movement(individual& i,
                  const landscape& my_landscape,
                  const vector<int> layer_nodes){
    if (i.smart() == 'y'){
        std::vector<double> attractiveness;
        std::vector<int> x_movement;
        std::vector<int> y_movement;
        for (double delta_x = -1; delta_x < 2; ++delta_x){
            for (double delta_y = -1; delta_y < 2; ++delta_y){

                vector<double> inputs = input_info(delta_x, delta_y, i, my_landscape);
                attractiveness.push_back(network_calc(layer_nodes, inputs, i.weights()));
                x_movement.push_back(delta_x);
                y_movement.push_back(delta_y);
            }
        }
        smart_movement(attractiveness, x_movement, y_movement, i, my_landscape);
    }
    else if (i.smart() == 'n'){
        const int sz{static_cast<int>(my_landscape.xsize())};
        const int sy{static_cast<int>(my_landscape.ysize())};
        // generate dist -1/1: random Movement
        std::uniform_int_distribution<> dist(-1, 1);

        assert(sz != 0 && sy != 0);
        i.setPosition(
                    (i.xposition() + dist(rng) + sz) % sz,
                    (i.yposition() + dist(rng) + sy) % sy
                    );
    }
}

///Iterate function ind_movement over entire population
void pop_movement (population& p,
                   const landscape& my_landscape,
                   const vector<int> layer_nodes){

    for (individual& i: p) { ind_movement(i, my_landscape, layer_nodes); }
}


///translates food intake into relative value over entire population, unequal fitness!
/// and substracts the energetic costs of the ANN
std::vector<double> collect_foods(population& p, const double ANN_cost)
{
    vector <double> food;
    food.reserve(p.size());
    for (int n = 0; n < static_cast<int>(p.size()); ++n) {
        if (p[n].smart() == 'y'){
            //assigns energy costs to ANN connections
            for (int o = 0; o < static_cast<int>(p[n].weights().size()); ++o){
                if (p[n].weights()[o] != 0){

                    p[n].food_update(ANN_cost);
                }
            }
        }

        food.push_back(p[n].return_food());
    }
    return food;
}

double calc_total_food(population& p, const double ANN_cost)
{
    const vector <double> food = collect_foods(p, ANN_cost);
    return std::accumulate(food.begin(), food.end(), 0.0);
}



std::vector<double> calculate_fitnesses_from_food(population& p, const double ANN_cost) {

    const double total_food{calc_total_food(p, ANN_cost)};
    std::vector<double> fitnesses = collect_foods(p, ANN_cost);
    for (double& fitness: fitnesses)
        fitness /= total_food;

    return fitnesses;
}



///Produces new weights after mutation
double produce_new_weight(individual& i, int weight_no){
    std::normal_distribution<double> dist(i.weights()[weight_no],0.5); //stdv 0.5!!
    return dist(rng);
}

///Mutates ANN weights
void mutation_i (individual& i, double prob_to_X, double prob_to_0){
    if (i.evolve() == 'y'){
        // random mutation event generation
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        for (int j = 0; j < static_cast<int>(i.weights().size()); ++j){
            double prob = dist(rng);
            if (prob < prob_to_X) {
                i.set_weight(j, produce_new_weight(i, j));
            }
            else if (prob > prob_to_X && prob < (prob_to_X + prob_to_0)){
                i.set_weight(j, 0);
            }
        }
    }
}


///loops over individuals in population
void mutation_all (population& p, double prob_to_X, double prob_to_0){
    for (individual& i: p) { mutation_i(i, prob_to_X, prob_to_0); }
}


///generates a new generation substituting the former.
///parents are assigned to offspring with their fitness value in relation to total fitness
void new_generation (population& p, std::vector<double> fitness_vector, int popsize){
    assert(p.size() > 0); //error if population is extinct!
    population offspring(popsize);

    for (int s = 0; s < popsize; ++s) { // loop over prey offspring
        // distribution offspring assignment
        std::uniform_real_distribution<double> dist(0.0, 1.0);

        double r1 = dist(rng);
        double prob = 0;
        //loop over parental generation
        for (int i = 0; i < static_cast<int>(p.size()); ++i) {

            prob += fitness_vector[i];

            if (r1 <= prob) {
                offspring[s] = p[i];
                break;
            }
        }
    }
    p = offspring;
}




///create a 2D landscape with dimensions x=n_cols and y=n_rows
landscape create_landscape(const int n_cols, const int n_rows)
{
    assert(n_cols >= 1);
    assert(n_rows >= 1);
    //X-Y-ordered
    vector<plot> plots;
    //landscape my_landscape(std::vector<plot>(n_rows * n_cols), n_cols);
    for (int row=0; row!=n_rows; ++row)
    {
        for (int col=0; col!=n_cols; ++col)
        {
            assert(col >= 0);
            //assert(col*row < static_cast<int>(my_landscape.size()));
            assert(row >= 0);
            //assert(row < static_cast<int>(my_landscape[col].size()));
            plot singleplot(row, col);
            std::uniform_real_distribution<double> dist1(0.0, 1.0);
            singleplot.setRisk(dist1(rng));
            plots.push_back(singleplot);
        }
    }
    landscape my_landscape(plots, n_cols);
    return my_landscape;
}



///increases the height of the grass
void let_grass_grow(landscape& Plots)
{
    for_plots(Plots, [](plot& p) { p.let_grass_grow(); } );
}

///Function to return neural complexity in population
void get_output(population& pop){
    double neural_complexity = 0;
    for (int i = 0; i < static_cast<int>(pop.size()); i++){
        for (int j = 0; j < static_cast<int>(pop[i].weights().size()); j++){
            if (pop[i].weights()[j] != 0){
                neural_complexity += 1.0/(pop.size() * pop[i].weights().size());
            }
        }
    }
    cout << "Neural complexity is " << neural_complexity*100 << "%" << endl
         << "population size after predation " << pop.size() << endl;

}





void do_simulation(cine_parameters parameter){
    //landscape is created
    landscape Plots = create_landscape(parameter.ncols(), parameter.nrows());
    //populations are created
    population prey(parameter.prey_pop());
    population predator(parameter.predator_pop());


    //positions and type initialization
    ini_positions(prey, parameter.prey_pop(),
                  parameter.ncols(), parameter.nrows(), 'h', 'y', 'y');
    ini_positions(predator, parameter.predator_pop(),
                  parameter.ncols(), parameter.nrows(), 'p', 'y', 'y');


    for (int g = 0; g < parameter.generations(); ++g) {

        for (int t = 0; t < parameter.timesteps(); ++t) {

            let_grass_grow(Plots);

            update_adclues(prey, predator, Plots);

            //Movements
            pop_movement(prey, Plots, parameter.layer_nodes());
            pop_movement(predator, Plots, parameter.layer_nodes());
            //Herbivores graze and deplete
            grazing(prey, Plots);
            //Predators hunt
            predation_simulation(prey, predator, Plots);
        }

        //Create fitness vectors for prey&predator based on collected food
        const std::vector<double> fitnesses_prey =
                calculate_fitnesses_from_food(prey, parameter.ANN_cost());
        const std::vector<double> fitnesses_predator =
                calculate_fitnesses_from_food(predator, parameter.ANN_cost());

        //Mutates ANN weights in population before reproduction
        mutation_all(prey, parameter.prob_mutation_to_rd(), parameter.prob_mutation_to_0());
        mutation_all(predator, parameter.prob_mutation_to_rd(), parameter.prob_mutation_to_0());

        get_output(prey);

        //generates new generation, inheritance of properties
        new_generation(prey, fitnesses_prey, parameter.prey_pop());
        new_generation(predator, fitnesses_predator, parameter.predator_pop());
    }
}


/* To Do:
 *
 * multiple herbivores on plot get grass/number
 *
 * Hard coded variables: movement +perception range, clue decay rate,
 *                       mutation sd, grass growth and threshold,
 *                       weight vector length and initial state,
 *                       population state, smart/random, evolvable/not
 *                       Noise ranges for Grass, risk and clues (H&P clues with equal noise)
 *
 * individual states 'type', 'smart' and 'evolve' take up too much(?)
 * computational power, move to population property?!

*/
