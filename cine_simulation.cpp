#include "cine_simulation.h"

#include <iostream>
#include <random>		//random number generation
#include <vector>		// for vector related commands
#include <numeric>		//needed for accumulate

using namespace std;

void terminateProgram()
{
    cout << "Press any character and <ENTER> to continue" << endl;
    char chAnyChar;
    cin >> chAnyChar;
    return;
    }




std::random_device rd;	// non-deterministic generator
std::mt19937 rng(rd());	// declare & seed a rng of type mersenne twister
std::uniform_real_distribution<double> dist1(0.1, 0.9);	// generate dist 0.1-0.9
std::uniform_int_distribution<> dist2(0, 10);	// generate dist 0/10
std::uniform_int_distribution<> dist3(-1, 1);	// generate dist -1/1
std::uniform_real_distribution<double> dist4(0, 1);




//define public functions
void plot::increaseGrass()
{
    if (dGrass < 1)
        dGrass += 0.2;

    if (dGrass > 1)
        dGrass = 1;
}

void plot::grazing()
{
    dGrass = 0;
}


void plot::setRisk()
{
    dRisk = dist1(rng);
}

void plot::setPosition(int iX, int iY)
{
    iXcoor = iX;
    iYcoor = iY;
}



//initialize class individual
class individual
{
public:
    void setPosition(int iX, int iY);

    int xposition() const { return iXcoor; }
    int yposition() const { return iYcoor; }
    void update_food(double dfoodintake);
    double return_food() const { return dfood; }
    void set_fitness(double dfit);
    double return_fitness() const { return dfitness;  }

private:
    double dfitness;
    double dfood;
    int iXcoor;
    int iYcoor;
};

//define public functions
void individual::setPosition(int iX, int iY){
    iXcoor = iX;
    iYcoor = iY;
}

void individual::update_food(double dfoodintake) {
    dfood += dfoodintake;
}

void individual::set_fitness(double dfit) {
    dfitness = dfit;
}

vector <double> dfoodV;	//initialize prey vector
vector <double> dfoodP;	//initialize predator vector
const int prey_pop = 25;
const int predator_pop = 25;
const int itimesteps = 10;
const int igenerations = 3;

//pulled out functions

void predation (int iSizeH, int iSizeP, int iSizepatch, individual H[], individual P[], plot patch[] ){

    for (int k = 0; k < iSizepatch; ++k){
        for (int l = 0; l < iSizeH; ++l){
            for (int m = 0; m < iSizeP; ++m) { // loop over predator individuals
                if (
                    patch[k].xposition() == H[l].xposition()
                    && H[l].xposition() == P[m].xposition()
                    && patch[k].yposition() == H[l].yposition()
                    && H[l].yposition() == P[m].yposition()
           ) {
                bernoulli_distribution bernoulli_d(patch[k].returnRisk());
            if (bernoulli_d(rng) == 1) {
                // prey gets eaten, fitness = 0, food intake prey
                P[m].update_food(1);
                H[l].set_fitness(0);
                H[l].setPosition(666, 666); // change
            }
            //else if (bernoulli_d(rng) == 0)
                // prey escapes, nothing happens / predator looses fitness?
        }

    }
    }
    }
}




void food_fitness (int iSize, individual xy[]){
    vector <double> dfood;	//initialize food vector
    for (int n = 0; n < iSize; ++n) {
        dfood.push_back(xy[n].return_food());			// ##scope?!
    }

    double dMean = accumulate(dfood.begin(), dfood.end(), 0.0);

    for (int q = 0; q < iSize; ++q) {
        if (xy[q].return_fitness() != 0)
            xy[q].set_fitness(xy[q].return_food() / dMean);
    }
}


void movement (int iSize, individual xy[], int dim){
    for (int m = 0; m < iSize; ++m){
    if (xy[m].return_fitness() != 0) {
        xy[m].setPosition(xy[m].xposition() + dist3(rng), xy[m].yposition() + dist3(rng));
        if (xy[m].xposition() > dim - 1)
            xy[m].setPosition(xy[m].xposition() - (dim - 1), xy[m].yposition());
        if (xy[m].xposition() < 0)
            xy[m].setPosition(dim - xy[m].xposition(), xy[m].yposition());
        if (xy[m].yposition() > dim - 1)
            xy[m].setPosition(xy[m].xposition(), xy[m].yposition() - (dim - 1));
        if (xy[m].yposition() < 0)
            xy[m].setPosition(xy[m].xposition(), dim - xy[m].yposition());
    }
   }
}


void reproduction(int iSize, individual xy[]){

    individual offspring[iSize];

    vector <double> dfit;	//initialize prey vector

    for (int t = 0; t < iSize; ++t) {
        dfit.push_back(xy[t].return_fitness());
    }

    double dtotalFit = accumulate(dfit.begin(), dfit.end(), 0.0);

    for (int u = 0; u < iSize; ++u) {
        dfit[u] = (dfit[u] / dtotalFit);
    }


    for (int s = 0; s < iSize; ++s) { // loop over prey offspring

        double r1 = dist4(rng);
        double prob = 0;

        for (int i = 0; i < iSize; ++i) {

            prob += dfit[i];

            if (r1 <= prob) {
                offspring[s] = xy[i];
                break;
            }
        }
    }
}

int edge = 10;

void do_simulation()
{
    //create a landscape
    plot Plots[edge * edge];
    for (int i = 0; i < edge * edge; ++i) {
        Plots[i].setRisk();						//random risk landscape
        Plots[i].setPosition(i / edge, i % edge);	//coordinates
    }

    individual prey[prey_pop];
    individual predator[predator_pop];
    for (int j = 0; j < prey_pop; ++j) {
        prey[j].setPosition(dist2(rng), dist2(rng));
    }

    for (int o = 0; o < predator_pop; ++o) {
        predator[o].setPosition(dist2(rng), dist2(rng));
    }

    for (int g = 0; g < igenerations; ++g) { //loop over generations

        for (int t = 0; t < itimesteps; ++t) { //loop over timesteps/movements
            for (int k = 0; k < edge * edge; ++k) { // loop over plots
                Plots[k].increaseGrass();	// Let grass grow
                for (int l = 0; l < prey_pop; ++ l) { // loop over prey individuals
                    if (Plots[k].xposition() == prey[l].xposition() && Plots[k].yposition() == prey[l].yposition()) {
                        // Attention: correct for two individuals on same plot
                        prey[l].update_food(Plots[k].dGrsupply());
                        Plots[k].grazing();
                    }
                }
            }

        predation(prey_pop, predator_pop, edge * edge, prey, predator, Plots);

        movement(prey_pop, prey, edge);
        movement(predator_pop, predator, edge);
        }
        // food to fitness: Calculate mean food intake, xi /mean equals the fitness
        food_fitness(prey_pop, prey);
        food_fitness(predator_pop, predator);

        //inheritance
        reproduction(prey_pop, prey); //returning updated prey pop?
        reproduction(predator_pop, predator); //returning updated predator pop?

    }
}


//implement hunger

//output stream: Positions? Complexity of ANN

//artificial neural network

