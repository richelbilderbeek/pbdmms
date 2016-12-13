#include "cine_simulation.h"

#include <iostream>
#include <cassert>
#include <random>		//random number generation
#include <vector>		// for vector related commands
#include <numeric>		//needed for accumulate
#include <functional>

using namespace std;


std::random_device rd;	// non-deterministic generator
std::mt19937 rng(rd());	// declare & seed a rng of type mersenne twister
std::uniform_real_distribution<double> dist1(0, 1);	// generate dist 0-1, risks
std::uniform_int_distribution<> dist2(0, 10);	// generate dist 0/10
std::uniform_int_distribution<> dist3(-1, 1);	// generate dist -1/1
std::uniform_real_distribution<double> dist4(0, 1);



vector <double> dfoodV;	//initialize prey vector
vector <double> dfoodP;	//initialize predator vector
//model parameters


//pulled out functions

void predation(population& H, population& P, const landscape& patch){

        for (unsigned int l = 0; l < H.size(); ++l){
            for (unsigned int m = 0; m < P.size(); ++m) { // loop over predator individuals
                if (
                        H[l].xposition() == P[m].xposition()
                        && H[l].yposition() == P[m].yposition()
                        ) {
                    bernoulli_distribution bernoulli_d(patch[P[m].xposition()][P[m].yposition()].returnRisk());
                    if (bernoulli_d(rng) == 1) {
                        // prey gets eaten, fitness = 0, food intake prey
                        P[m].update_food(1);
                        //H[l].set_fitness(0);
                        //H[l].setPosition(666, 666); // change
                        H[l] = H.back();
                        H.pop_back();
                        --l; //Dangerous!
                    }
                    //else if (bernoulli_d(rng) == 0)
                    // prey escapes, nothing happens / predator looses fitness?
                }

            }
        }
}

// move on grid, now random
//ADAPT TO ANN (movement based on probabilities derived from patch attractivity
void movement (population& p, const landscape& my_landscape){
  for (individual& i: p) { movement(i, my_landscape); }
}

void movement (individual& i, const landscape& my_landscape){
    const int sz{static_cast<int>(my_landscape.size())};
    assert(sz != 0);
    assert(my_landscape.size() == my_landscape[0].size());
    i.setPosition(
      (i.xposition() + dist3(rng) + sz) % sz,
      (i.yposition() + dist3(rng) + sz) % sz
    );
}



//translates food intake into relative value over entire population, unequal fitness!
std::vector<double> collect_foods(const population& xy)
{
    vector <double> food;
    food.reserve(xy.size());
    for (unsigned int n = 0; n < xy.size(); ++n) {
        food.push_back(xy[n].return_food());
    }
    return food;
}

double calc_total_food(const population& xy)
{
    const vector <double> food = collect_foods(xy);
    return std::accumulate(food .begin(), food .end(), 0.0);
}

std::vector<double> calculate_fitnesses_from_food(const population& xy) {

    const double total_food{calc_total_food(xy)};
    std::vector<double> fitnesses = collect_foods(xy);
    for (double& fitness: fitnesses)
        fitness /= total_food;

return fitnesses;
}


/*

void assign_fitness_from_food(population& xy) {

    const double mean_food{calc_mean_food(xy)};
    for (unsigned int q = 0; q < xy.size(); ++q) {
        //if (xy[q].return_fitness() != 0)
            xy[q].set_fitness(xy[q].return_food() / mean_food);
    }
}   */


void reproduction (population& p, std::vector<double> fitv){
    population offspring(p.size());

    double total_Fit = accumulate(fitv.begin(), fitv.end(), 0.0);

    for (unsigned int u = 0; u < p.size(); ++u) {
        fitv[u] /= total_Fit;
    }


    for (unsigned int s = 0; s < p.size(); ++s) { // loop over prey offspring

        double r1 = dist4(rng);
        double prob = 0;

        for (unsigned int i = 0; i < p.size(); ++i) {

            prob += fitv[i];

            if (r1 <= prob) {
                offspring[s] = p[i];
                break;
            }
        }
    }
}



/*
void reproduction(int size, individual xy[]){

    individual offspring[size];

    vector <double> fitv;	//initialize prey vector

    for (int t = 0; t < size; ++t) {
        fitv.push_back(xy[t].return_fitness());
    }

    double total_Fit = accumulate(fitv.begin(), fitv.end(), 0.0);

    for (int u = 0; u < size; ++u) {
        fitv[u] = (fitv[u] / total_Fit);
    }


    for (int s = 0; s < size; ++s) { // loop over prey offspring

        double r1 = dist4(rng);
        double prob = 0;

        for (int i = 0; i < size; ++i) {

            prob += fitv[i];

            if (r1 <= prob) {
                offspring[s] = xy[i];
                break;
            }
        }
    }
}
*/
//int edge = 10;

//assesses attractivity to surrounding plots
void ANN_assessment(population& p){

    for (unsigned int individual_index = 0; individual_index < p.size(); ++individual_index){
        for (int delta_x = -1; delta_x < 2; ++delta_x){
            for (int delta_y = -1; delta_y < 2; ++delta_y){
                /*
                for (int l = 0; l < sizepatch; ++l){
                    if(patch[l].xposition() == (xy[individual_index].xposition() + delta_x) && patch[l].yposition() == (xy[individual_index].yposition() + delta_y))
                        inputs.col(0) = float(patch[l].dGrsupply()); inputs.col(1) = float(patch[l].returnRisk());

                    for (int m = 0; m < sizeadv; ++m){
                        if(adv[m].xposition() == (xy[individual_index].xposition() + delta_x) && adv[m].yposition() == (xy[individual_index].yposition() + delta_y))
                            inputs.col(3) = float(1);
                        else
                            inputs.col(3) = float(0);
                    }
                }
                */
            }
        }
    }

}

const int timesteps = 10;
const int generations = 3;
const int prey_pop = 25;
const int predator_pop = 25;

landscape create_landscape(const int n_cols, const int n_rows)
{
  assert(n_cols >= 1);
  assert(n_rows >= 1);
  //X-Y-ordered
  landscape my_landscape(n_cols, std::vector<plot>(n_rows, plot(0,0)));
  for (int row=0; row!=n_rows; ++row)
  {
      for (int col=0; col!=n_cols; ++col)
      {
          assert(col >= 0);
          assert(col < static_cast<int>(my_landscape.size()));
          assert(row >= 0);
          assert(row < static_cast<int>(my_landscape[col].size()));
          my_landscape[col][row] = plot(row, col);
      }
  }
  return my_landscape;
}

//Applies a funcction to all elements of the landscape
void for_each(landscape& my_landscape, std::function<void(plot&)> f)
{
    for (int i = 0; i < static_cast<int>(my_landscape.size()); ++i)
    {
        for (int j = 0; j < static_cast<int>(my_landscape[0].size()); ++j)
        {
            assert(i >= 0);
            assert(i < static_cast<int>(my_landscape.size()));
            assert(j >= 0);
            assert(j < static_cast<int>(my_landscape[i].size()));
            f(my_landscape[i][j]);
        }
    }
}

void let_grass_grow(landscape& Plots)
{
  for_each(Plots, [](plot& p) { p.let_grass_grow(); } );
}

/*
void let_grass_grow(landscape& Plots)
{
    for (int i = 0; i < static_cast<int>(Plots.size()); ++i)
    {
        for (int j = 0; j < static_cast<int>(Plots[0].size()); ++j)
        {
            assert(i >= 0);
            assert(i < static_cast<int>(Plots.size()));
            assert(j >= 0);
            assert(j < static_cast<int>(Plots[i].size()));
            Plots[i][j].let_grass_grow();
        }
    }
}
*/

void do_simulation(const int n_cols, const int n_rows)
{
    //create a landscape

    //landscape Plots(n_cols, column);
    landscape Plots = create_landscape(n_cols, n_rows);

    for_each(Plots, [](plot& p) { p.setRisk(dist1(rng)); } );

    population prey(prey_pop);
    population predator(predator_pop);
    for (int j = 0; j < prey_pop; ++j) {
        prey[j].setPosition(dist2(rng), dist2(rng));
    }

    for (int o = 0; o < predator_pop; ++o) {
        predator[o].setPosition(dist2(rng), dist2(rng));
    }

    for (int g = 0; g < generations; ++g) { //loop over generations

        for (int t = 0; t < timesteps; ++t) { //loop over timesteps/movements

            let_grass_grow(Plots);

                for (int l = 0; l < prey_pop; ++ l) { // loop over prey individuals
                        // Attention: correct for two individuals on same plot
                        prey[l].update_food(Plots[prey[l].xposition()][prey[l].yposition()].dGrsupply());
                        Plots[prey[l].xposition()][prey[l].yposition()].grass_consumption();

                }


        predation(prey, predator, Plots);

        movement(prey, Plots);
        movement(predator, Plots);
        }
        // food to fitness: Calculate mean food intake, xi /mean equals the fitness
        //assign_fitness_from_food(prey);
        //assign_fitness_from_food(predator);
        const std::vector<double> fitnesses_prey = calculate_fitnesses_from_food(prey);
        const std::vector<double> fitnesses_predator = calculate_fitnesses_from_food(predator);

        //inheritance
        reproduction(prey, fitnesses_prey); //returning updated prey pop?
        reproduction(predator, fitnesses_predator); //returning updated predator pop?
        //reproduction(prey); //returning updated prey pop?
        //reproduction(predator); //returning updated predator pop?

    }
}


//implement hunger

//output stream: Positions? Complexity of ANN


//replace arrays with vectors consistently

//clearer naming

//no global constants

//utilize more efficient way of position determination!

