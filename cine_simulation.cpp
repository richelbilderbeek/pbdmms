#include "cine_simulation.h"
#include <iostream>
#include <cassert>
#include <random>		//random number generation
#include <vector>		// for vector related commands
#include <numeric>		//needed for accumulate
#include <functional>
#include <fstream>
#include <math.h>
#include <string>
#include <opencv/cv.h>
#include <opencv/ml.h>
#include <opencv/highgui.h>
//#include <typeinfo>




using namespace cv;
using namespace std;


std::random_device rd;                              // non-deterministic generator
std::mt19937 rng(rd());                             // declare & seed a rng of type mersenne twister
std::uniform_real_distribution<double> dist1(0, 1);	// generate dist 0-1, pred. risk on patch
std::uniform_int_distribution<> dist2(0, 9);        // generate dist 0-9, init pos of ind.
std::uniform_int_distribution<> dist3(-1, 1);       // generate dist -1/1: Movement
std::uniform_real_distribution<double> dist4(0, 1); // assignment of offspring, combine with dist1?
std::uniform_real_distribution<float> dist5(0, 1); // movement, combine with dist1?
std::uniform_real_distribution<double> dist6(0, 1); // weight mutation



vector <double> dfoodV;	//initialize prey vector
vector <double> dfoodP;	//initialize predator vector


///constants
/*
const int timesteps = 10;
const int generations = 3;
const int prey_pop = 25;
const int predator_pop = 25;
const float prob_mutation_to_0 = 0.05;
const float prob_mutation_to_rd = 0.025;
*/
///Functions///

///simulates predation. If predator and prey occupy same patch,
///predation is successfull with probability m_Risk.
/// Predation results in deletion of prey individual & food_uptake by predator
void predation_simulation(population& H, population& P, const landscape& patch){

    for (int l = 0; l < static_cast<int>(H.size()); ++l){
        for (unsigned int m = 0; m < P.size(); ++m) { // loop over predator individuals


            assert(l <= static_cast<int>(H.size()));

            if (H[l].xposition() == P[m].xposition()
                    && H[l].yposition() == P[m].yposition()
                    ) {
                bernoulli_distribution
                        bernoulli_d(patch[P[m].xposition()][P[m].yposition()].returnRisk());
                if (bernoulli_d(rng) == 1) {    //i.e. if prey is caught
                    P[m].food_uptake(1);        //1 prey item is added to
                    H[l] = H.back();
                    H.pop_back();
                    --l; //Dangerous!
                }

            }

        }
    }
}


bool is_regular_file(const std::string& filename) noexcept
{
  std::fstream f;
  f.open(filename.c_str(),std::ios::in);
  return f.is_open();
}

std::vector<std::string> file_to_vector(const std::string& filename)
{
  if(!is_regular_file(filename))
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "can only convert existing files, "
      << "filename supplied: '"
      << filename << "' was not found"
    ;
    throw std::invalid_argument(msg.str());
  }
  assert(is_regular_file(filename));
  std::vector<std::string> v;
  std::ifstream in{filename.c_str()};
  assert(in.is_open());
  //Without this test in release mode,
  //the program might run indefinitely when the file does not exists
  for (int i = 0; !in.eof(); ++i)
  {
    std::string s;
    std::getline(in,s);
    v.push_back(s); //Might throw std::bad_alloc
  }
  //Remove empty line at back of vector
  if (!v.empty() && v.back().empty()) v.pop_back();
  return v;
}


///update ANN to use individual's weights
void setup_ANN(individual& i){

    std::vector<float> fweights = i.return_weightvct();
    ofstream b_file( "cine_mlp_1.yml");

    //Write all lines from template file to the output file
    for (const auto& line: file_to_vector("cine_mlp_template.yml"))
    {
      b_file << line << '\n';
    }

    b_file << "   weights:" << endl << "      - [ ";
    for (int j = 0; j < 16; ++j){
        if (j == 11)
            b_file << fweights[j] << " ]" << endl << "      - [ ";
        if (j == 15)
            b_file << fweights[j] << " ]";
        else
            b_file << fweights[j] << ", ";
    }
}



///returns input information for ANN
cv::Mat input_info(int delta_x, int delta_y,
                   individual& i,
                   const landscape& my_landscape,
                   const population& adv){

    const int sz{static_cast<int>(my_landscape.size())};
    const int sy{static_cast<int>(my_landscape[0].size())};


    int pos_x = (i.xposition() + delta_x + sz) % sz;
    int pos_y = (i.yposition() + delta_y + sy) % sy;


    plot patch1 = my_landscape[pos_x][pos_y];

    cv::Mat inputs = cv::Mat(1, 3, CV_32FC1);

    inputs.col(0) = float(patch1.grass_height());
    inputs.col(1) = float(patch1.returnRisk());

    float adv_count = 0;
    for (unsigned int m = 0; m < adv.size(); ++m){
        if(adv[m].xposition() == patch1.xposition() && adv[m].yposition() == patch1.yposition())
            ++adv_count;
    }

    inputs.col(2) = adv_count;

    return inputs;
}


///takes input to ANN and calculates plot attractivity
float ANN_calculation(cv::Mat& inputs){
    CvANN_MLP mlp;
    mlp.load("cine_mlp_1.yml","mlp");

    cv::Mat response;
    mlp.predict(inputs, response);
    return response.at<float>(0,0);
}


///Normalize attractiveness values
void calc_relative_attractiveness (std::vector<float>& attractiveness){

    float att_total;
    att_total= std::accumulate(attractiveness.begin(), attractiveness.end(), 0.0);
    for (unsigned int l = 0; l < attractiveness.size(); ++l){
        attractiveness[l] /= att_total;
    }
}



///move based on attractivity values
void smart_movement (std::vector<float>& attractiveness,
                     std::vector<int>& x_movement,
                     std::vector<int>& y_movement,
                     individual& i, landscape my_landscape){

    const int sz{static_cast<int>(my_landscape.size())};
    const int sy{static_cast<int>(my_landscape[0].size())};
    assert(sz != 0 && sy != 0);

    calc_relative_attractiveness(attractiveness);

    float r2 = dist5(rng);
    float prob = 0;

    for (unsigned int j = 0; j < attractiveness.size(); ++j) {

        prob += attractiveness[j];

        if (r2 <= prob) {
            i.setPosition(
                        (i.xposition() + x_movement[j] + sz) % sz,
                        (i.yposition() + y_movement[j] + sy) % sy
                        );
            break;
        }
    }

}


///makes use of above funcitons to let an individual move directed by ANN
void input_to_movement(individual& i, const landscape& my_landscape, const population& adv){
    //setup_ANN(i);
    std::vector<float> attractiveness;
    std::vector<int> x_movement;
    std::vector<int> y_movement;
    for (float delta_x = -1; delta_x < 2; ++delta_x){
        for (float delta_y = -1; delta_y < 2; ++delta_y){

            cv::Mat inputs = input_info(delta_x, delta_y, i, my_landscape, adv);
            attractiveness.push_back(ANN_calculation(inputs));
            x_movement.push_back(delta_x);
            y_movement.push_back(delta_y);
        }
    }
smart_movement(attractiveness, x_movement, y_movement, i, my_landscape);
}

///Iterate function input_to_movement over entire population
void smart_pop_movement (population& p, const landscape& my_landscape, const population& adv){
    for (individual& i: p) { input_to_movement(i, my_landscape, adv); }
  }



/// move on grid, now random
void random_movement (population& p, const landscape& my_landscape){
  for (individual& i: p) { random_movement(i, my_landscape); }
}

void random_movement (individual& i, const landscape& my_landscape){
    const int sz{static_cast<int>(my_landscape.size())};
    const int sy{static_cast<int>(my_landscape[0].size())};
    assert(sz != 0 && sy != 0);
    //assert(my_landscape.size() == my_landscape[0].size());
    i.setPosition(
      (i.xposition() + dist3(rng) + sz) % sz,
      (i.yposition() + dist3(rng) + sy) % sy
    );
}


///translates food intake into relative value over entire population, unequal fitness!
std::vector<double> collect_foods(const population& p)
{
    vector <double> food;
    food.reserve(p.size());
    for (unsigned int n = 0; n < p.size(); ++n) {
        food.push_back(p[n].return_food());
    }
    return food;
}

double calc_total_food(const population& p)
{
    const vector <double> food = collect_foods(p);
    return std::accumulate(food .begin(), food .end(), 0.0);
}

std::vector<double> calculate_fitnesses_from_food(const population& p) {

    const double total_food{calc_total_food(p)};
    std::vector<double> fitnesses = collect_foods(p);
    for (double& fitness: fitnesses)
        fitness /= total_food;

return fitnesses;
}



///Produces new weights after mutation
float produce_new_weight(individual& i, int weight_no){
      std::normal_distribution<float> distribution(i.return_weight(weight_no),0.5); //stdv 0.5!!
      return distribution(rng);
}

///Mutates ANN weights
void mutation_i (individual& i, float probability){
    for (int j = 0; j < i.return_weightlength(); ++j){
        if (dist6(rng) < probability) {
            if(probability == prob_mutation_to_rd){
                i.set_weight(j, produce_new_weight(i, j));

            }
            else if (probability == prob_mutation_to_0){
                i.set_weight(j, 0);

            }
        }
    }
}



void mutation_all (population& p, float probability){
  for (individual& i: p) { mutation_i(i, probability); }
}


///generates a new generation substituting the former.
///parents are assigned to offspring with their fitness value in relation to total fitness
void new_generation (population& p, std::vector<double> fitness_vector){
    population offspring(p.size());

    for (unsigned int s = 0; s < p.size(); ++s) { // loop over prey offspring

        double r1 = dist4(rng);
        double prob = 0;

        for (unsigned int i = 0; i < p.size(); ++i) {

            prob += fitness_vector[i];

            if (r1 <= prob) {
                offspring[s] = p[i];
                break;
            }
        }
    }
}




///create a 2D landscape with dimensions x=n_cols and y=n_rows
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

///Applies a function to all elements of the landscape
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


///increases the height of the grass
void let_grass_grow(landscape& Plots)
{
  for_each(Plots, [](plot& p) { p.let_grass_grow(); } );
}







void do_simulation(const int n_cols, const int n_rows)
{
    landscape Plots = create_landscape(n_cols, n_rows);//landscape is created
    for_each(Plots, [](plot& p) { p.setRisk(dist1(rng)); } );//risk is assigned

    population prey(prey_pop);          //create prey population with size prey_pop
    population predator(predator_pop);  //create predator population with size predator_pop

    //assign positions to prey&predator
    for (int j = 0; j < prey_pop; ++j) {
        prey[j].setPosition(dist2(rng), dist2(rng));
    }
    for (int o = 0; o < predator_pop; ++o) {
        predator[o].setPosition(dist2(rng), dist2(rng));
    }

    for (int g = 0; g < generations; ++g) {     //loop over generations
        for (int t = 0; t < timesteps; ++t) {   //loop over timesteps/movements
            let_grass_grow(Plots);              //grass grows
            // loop over prey individuals
            for (int l = 0; l < static_cast<int>(prey.size()); ++ l) {
                // Attention: correct for two individuals on same plot
                //prey takes up food from currently occupied plot
                prey[l].food_uptake(Plots[prey[l].xposition()][prey[l].yposition()].grass_height());
                //consumed grass is depleted from plot
                Plots[prey[l].xposition()][prey[l].yposition()].grass_consumption();
            }

            predation_simulation(prey, predator, Plots);//simulates predation events

            //prey moves on landscape Plots
            random_movement(prey, Plots);
            smart_pop_movement(predator, Plots, prey);
        }

        //Create fitness vectors for prey&predator based on collected food
        const std::vector<double> fitnesses_prey = calculate_fitnesses_from_food(prey);
        const std::vector<double> fitnesses_predator = calculate_fitnesses_from_food(predator);

        //Mutates ANN weights in population before reproduction
        mutation_all(prey, prob_mutation_to_0);
        mutation_all(prey, prob_mutation_to_rd);
        mutation_all(predator, prob_mutation_to_0);
        mutation_all(predator, prob_mutation_to_rd);

        //generates new generation, inheritance of properties
        new_generation(prey, fitnesses_prey);
        new_generation(predator, fitnesses_predator);
    }
}


//implement hunger

//output stream: Positions? Complexity of ANN


//clearer naming

//no global constants


