/*	jaan_main.cpp
    Written by James Andrews on 17-10-2016
    A simulation of a population with sexual selection.
    Based on Cichlids in Lake Victoria.

    ASSUMPTIONS
    Haploid, hermaphroditic population
    Multiple loci for trait and preference.
    Sequential, fixed interval sampling in mate selection.
    Mate selection prefers bigger traits.
    Fixed, discrete generations.

    From here:
     - Weighted lottery for who is a mother based on vFemale.
     - Weighted lottery for who is a father based on vMale.
     - Mate choice expression exp(p(focal mother) * t(all fathers) * alpha(all fathers), who
       is best choice is when this expression is maximised but then she chooses based on this
       distribution. This changes mate selection to match preference.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include "jaan_parameters.h"

//

// CLASS SECTION

//

class Individual
// Individual class declaration.
{
public:
    Individual(const jaan_parameters& p, std::mt19937& generator);
    Individual(const Individual&, const Individual&, const jaan_parameters& p, std::mt19937& generator);
    void mateSelect(const std::vector<Individual>& population, const int dead, const jaan_parameters& p, std::mt19937& generator);
    void mortality(const jaan_parameters& p, std::mt19937& generator);
    double getPref();
    double getTrt();
    int getMate();
    bool alive;
    double vFemale;
    double vFcum;
    double vMale;
    double vMcum;
private:
    void mutate(const jaan_parameters& p, std::mt19937& generator);
    void develop(const jaan_parameters& p);
    std::vector<double> prefGenes;
    std::vector<double> trtGenes;
    double preference;
    double trait;
    int mate;
};

// PUBLIC INDIVIDUAL CLASS FUNCTIONS

// INDIVIDUAL CLASS CONSTRUCTORS
Individual::Individual(const jaan_parameters& p, std::mt19937& generator) :
// Initialisation constructor.
alive(true),
vFemale(0.0),
vFcum(0.0),
vMale(0.0),
vMcum(0.0),
prefGenes(p.nPrefGenes),
trtGenes(p.nTrtGenes),
preference(0.0),
trait(0.0),
mate(-1)
{
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    for (int i = 0; i < p.nTrtGenes; ++i) {       // Randomly set each gene to 0 or 1.
        if (distribution(generator) < 0.2) {
            trtGenes[i] = 0;
        }
        else {
            trtGenes[i] = 1;
        }
    }
    for (int i = 0; i < p.nPrefGenes; ++i) {      // Randomly set each Preference gene to 0 or 1.
        if (distribution(generator) < 0.8) {
            prefGenes[i] = 0;
        }
        else {
            prefGenes[i] = 1;
        }
    }
    mutate(p, generator);
    develop(p);
}

Individual::Individual(const Individual& mother, const Individual& father, const jaan_parameters& p, std::mt19937& generator) :
// Reproduction constructor.
alive(true),
vFemale(0.0),
vFcum(0.0),
vMale(0.0),
vMcum(0.0),
prefGenes(p.nPrefGenes),
trtGenes(p.nTrtGenes),
preference(0.0),
trait(0.0),
mate(-1)
{
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    for (int i = 0; i < p.nTrtGenes; ++i) {          // Inherit mother and father genes randomly 50:50.
        if (distribution(generator) < 0.5) {
            trtGenes[i] = mother.trtGenes[i];
        }
        else {
            trtGenes[i] = father.trtGenes[i];
        }
    }
    for (int i = 0; i < p.nPrefGenes; ++i) {         // Inherit mother and father preference genes randomly 50:50.
        if (distribution(generator) < 0.5) {
            prefGenes[i] = mother.prefGenes[i];
        }
        else {
            prefGenes[i] = father.prefGenes[i];
        }
    }
    mutate(p, generator);
    develop(p);
}

// CLASS FUNCTIONS
void Individual::mateSelect(const std::vector<Individual>& population, const int dead, const jaan_parameters& p, std::mt19937& generator)
/*	Function for females to find a partner.
    Takes a vector of all individuals and selects randomly from the second half of the vector.
    Function returns -1 if no suitable mate is found, otherwise returns the position of the male. */
{
    for (double t = 0.0; t < p.popSize - dead; ++t) {                           // Sample the males at random.
        std::uniform_int_distribution<int> pickMan(0, p.popSize - dead - 1);
        int focal = pickMan(generator);

        /* ################################
         *
         * Need to test whether focal individual is me, if so, pick another
         *
         * ################################
         */

        if (population[focal].trait >= preference) {
            mate = focal;                                                          // If the male is good, return his position in vector.
        }
    }
    if (mate == -1) {
        alive = false;
    }
}

void Individual::mortality(const jaan_parameters& p, std::mt19937& generator)
// Causes mortality for males based on trait cost.
{
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    if (trait * p.traitCost > distribution(generator)) {
        alive = false;
    }
}

int Individual::getMate()
{
    return mate;
}

double Individual::getPref()
{
    return preference;
}

double Individual::getTrt()
{
    return trait;
}

//PRIVATE INDIVIDUAL CLASS FUNCTIONS
void Individual::mutate(const jaan_parameters& p, std::mt19937& generator)
// Give each gene a chance of flipping.
{
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    for (int i = 0; i < p.nTrtGenes; ++i) {
        if (distribution(generator) < p.mu)  {
            if (trtGenes[i] == 1) {
                trtGenes[i] = -1;
            }
            else trtGenes[i] = 1;
        }
    }
    for (int i = 0; i < p.nPrefGenes; ++i) {
        if (distribution(generator) < p.mu) {
            if (prefGenes[i] == 1) {
                prefGenes[i] = -1;
            }
            else prefGenes[i] = 1;
        }
    }
}

void Individual::develop(const jaan_parameters& p)
/*	Calculate preference from prefGenes.
    Calculate trait from trtGenes.*/
{
    for (int i = 0; i < p.nPrefGenes; ++i) {
        preference += prefGenes[i];
    }
    preference /= p.nPrefGenes;
    double temp = (preference - p.pOpt) / p.deltap;
    vFemale = exp(-0.5 * temp * temp);

    for (int i = 0; i < p.nTrtGenes; ++i) {
        trait += trtGenes[i];
    }
    trait /= p.nTrtGenes;
    temp = (trait - p.tOpt) / p.deltat;
    vMale = exp(-0.5 * temp * temp);
}

//

// PROGRAM SECTION

//

void terminateProgram()
// Wait for user input before closing.
{
    std::cout << "Done!" << std::endl;
//    char chAnyChar;
//    std::cin >> chAnyChar;
    return;
}

void arrangeMales(const int &dead, std::vector<Individual> &population, const jaan_parameters& p)
// Swaps dead males with alive ones so that they are sorted into two sections.
{
    for (int i = 0; i < p.popSize - dead; ++i) {
        if (!population[i].alive) {
            for (int j = p.popSize - dead; j < p.popSize; ++j) {
                if (population[j].alive) {
                    std::swap(population[i], population[j]);
                    break;
                }
            }
        }
    }
}

int main()
{
    jaan_parameters p;
    std::mt19937 generator;                          // Random number generator engine for putting into the uniform distribution functions.
    // Set up initial population.
    std::vector<Individual> population(p.popSize, Individual(p, generator));

    //	Enter generational For loop.
    for (int g = 0; g < p.gEnd; ++g) {

        // Mortality of males by trait size.
        for (int i = 0; i < p.popSize; ++i) {
            population[i].mortality(p, generator);
        }
        // Rearrange Population.
        int dead = 0;
        for (int i = 0; i < p.popSize; ++i) {
            if (population[i].alive == false) {
                ++dead;
            }
        }
        if (dead == p.popSize) {
            std::cerr << "All dead.";
            exit(1);
        }
        arrangeMales(dead, population, p);

        // Female mate choice.
        // Females select mates and those that fail to reproduce are moved to the end of the vector.
        double cum_viab = 0.0;
        for (int i = 0; i < p.popSize; ++i) {
            population[i].mateSelect(population, dead, p, generator);
            cum_viab = population[i].vFemale;    // Cumulative probabilty variable.
            population[i].vFcum = cum_viab;      // Cumulative probability up to individual i.
        }
        std::uniform_real_distribution<double> pickMother(0, cum_viab); // Create a uniform distribution up to the size of the cumulative probability.

        // Rearrange population once more, to ensure that females that didn't reproduce are at the end. important for when females don't sample the entire population.
        dead = 0;
        for (int i = 0; i < p.popSize; ++i) {
            if (population[i].alive == false) {
                ++dead;
            }
        }
        if (dead == p.popSize) {
            std::cerr << "All dead.";
            exit(1);
        }
        arrangeMales(dead, population, p);

        // Matings
        std::vector<Individual> offspring;
        for (int i = 0; i < p.popSize; ++i) {
            double chosen = pickMother(generator); // Choose a random number from the probability distribution.
            int mother = -1;
            for (int i = 0; i < p.popSize; ++i) {
                if (i == p.popSize - 1) { // If you hit the end of the vector, the last individual is the mother.
                    mother = i;
                }
                else if (population[i + 1].vFcum > chosen) { // Otherwise, compare the random number to the cumulative probability up to the next individual, if that individual has a higher probability distribution, then this individual is the mother.
                    mother = i;
                }
            }
            if (mother == -1) {
                std::cerr  << "Problem with setting mother.";
                exit(1);
            }
            // Set the mother to the current female and the father to the chosen mate of the female.
            int father = population[mother].getMate();
            // If the mother is dead, choose a random female from the living females and her partner.
            if (population[mother].alive == 0) {
                std::uniform_int_distribution<int> pickWoman(0, p.popSize - dead - 1);
                mother = pickWoman(generator);
                father = population[mother].getMate();
            }
            if (father < 0) {
                std::cerr << "PROBLEM WITH MATING\n";
            }
            // Produce two offspring and attach one each to the vectors for males and females.
            Individual child(population[mother], population[father], p, generator);
            offspring.push_back(child);
        }

        // Set offspring as the current generation.
        population = offspring;
    }

    // Terminate Program.
    terminateProgram();
    return 0;
}
