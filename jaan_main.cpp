/*	jaan_main.cpp
    Written by James Andrews on 17-10-2016
    A simulation of a population with sexual selection.
    Based on Cichlids in Lake Victoria.

    ASSUMPTIONS
    Haploid population
    Multiple loci with weak increase on trait and preference.
    Sequential, fixed interval sampling in mate selection.
    Mate selection prefers bigger traits.
    Fixed, discrete generations.

    From here:
     - Switch to hermaphrodites
     - Weighted lottery for who is a mother Vf = exp(-0.5*((p-popt)/deltap)^2) chosen from
       population based on this distribution.
     - Weighted lottery for who is a father Vm = exp(-0.5*((t-topt)/deltat)^2) chosen from
       population based on this distribution.
     - Mate choice expression exp(p(focal mother) * t(all fathers) * alpha(all fathers), who
       is best choice is when this expression is maximised but then she chooses based on this
       distribution.
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>

const int gEnd          = 1000;             // Number of Generations to run through.
const int popSize       = 1000;             // Population size.
const int nMales        = popSize / 2;      // How many individuals in a vector are treated as males.
const int nFemales      = popSize - nMales; // How many individuals in a vector are treated as females.
const int nPrefGenes    = 10;               // Number of genes for Preference.
const int nTrtGenes     = 10;               // Number of genes for Trait.
const double traitCost  = 0.0;              // How dangerous it is to have an Trait.
const double pOpt       = 0.0;              // Optimal preference value for viability.
const double tOpt       = 0.0;              // Optimal trait value for viability.
const double deltap     = 0.0;              // Cost of choosiness.
const double deltat     = 0.0;              // Cost of trait.
const double mu         = 1.0e-4;           // Mutation rate.
std::default_random_engine generator;
std::uniform_real_distribution<double> distribution(0.0, 1.0);

//

// CLASS SECTION

//

class Individual
// Individual class declaration.
{
public:
    Individual();
    Individual(const Individual&, const Individual&);
    void mateSelect(const std::vector<Individual>&, const int);
    void mortality(const double traitCost);
    double getPref();
    double getTrt();
    int getMate();
    bool alive;
private:
    void mutate();
    void develop();
    std::vector<double> prefGenes;
    std::vector<double> trtGenes;
    double preference;
    double trait;
    double vFemale;
    double vMale;
    int mate;
};

// PUBLIC INDIVIDUAL CLASS FUNCTIONS

// INDIVIDUAL CLASS CONSTRUCTORS
Individual::Individual() :
// Initialisation constructor.
alive(true),
prefGenes(nPrefGenes),
trtGenes(nTrtGenes),
preference(0.0),
trait(0.0),
vFemale(0.0),
vMale(0.0),
mate(-1)
{
    for (int i = 0; i < nTrtGenes; ++i) {       // Randomly set each gene to 0 or 1.
        if (distribution(generator) < 0.2) {
            trtGenes[i] = 0;
        }
        else {
            trtGenes[i] = 1;
        }
    }
    for (int i = 0; i < nPrefGenes; ++i) {      // Randomly set each Preference gene to 0 or 1.
        if (distribution(generator) < 0.8) {
            prefGenes[i] = 0;
        }
        else {
            prefGenes[i] = 1;
        }
    }
    mutate();
    develop();
}

Individual::Individual(const Individual& mother, const Individual& father) :
// Reproduction constructor.
alive(true),
prefGenes(nPrefGenes),
trtGenes(nTrtGenes),
preference(0.0),
trait(0.0),
vFemale(0.0),
vMale(0.0),
mate(-1)
{
    for (int i = 0; i < nTrtGenes; ++i) {          // Inherit mother and father genes randomly 50:50.
        if (distribution(generator) < 0.5) {
            trtGenes[i] = mother.trtGenes[i];
        }
        else {
            trtGenes[i] = father.trtGenes[i];
        }
    }
    for (int i = 0; i < nPrefGenes; ++i) {         // Inherit mother and father preference genes randomly 50:50.
        if (distribution(generator) < 0.5) {
            prefGenes[i] = mother.prefGenes[i];
        }
        else {
            prefGenes[i] = father.prefGenes[i];
        }
    }
    mutate();
    develop();
}

// CLASS FUNCTIONS
void Individual::mateSelect(const std::vector<Individual>& population, const int deadMales)
/*	Function for females to find a partner.
    Takes a vector of all individuals and selects randomly from the second half of the vector.
    Function returns -1 if no suitable mate is found, otherwise returns the position of the male. */
{
    for (double t = 0.0; t < nMales; ++t) {                                     // Sample the males at random.
        std::uniform_int_distribution<int> pickMan(0, nMales - deadMales - 1);
        int focal = nMales + pickMan(generator);

        if (population[focal].trait >= preference) {
            mate = focal;                                                       // If the male is good, return his position in vector.
        }
    }
    if (mate == -1) {
        alive = false;
    }
}

void Individual::mortality(const double traitCost)
// Causes mortality for males based on trait cost.
{
    if (trait * traitCost > distribution(generator)) {
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
void Individual::mutate()
// Give each gene a chance of flipping.
{
    for (int i = 0; i < nTrtGenes; ++i) {
        if (distribution(generator) < mu)  {
            if (trtGenes[i] == 1) {
                trtGenes[i] = 0;
            }
            else trtGenes[i] = 1;
        }
    }
    for (int i = 0; i < nPrefGenes; ++i) {
        if (distribution(generator) < mu) {
            if (prefGenes[i] == 1) {
                prefGenes[i] = 0;
            }
            else prefGenes[i] = 1;
        }
    }
}

void Individual::develop()
/*	Calculate preference from prefGenes.
    Calculate trait from trtGenes.*/
{
    for (int i = 0; i < nPrefGenes; ++i) {
        preference += prefGenes[i];
    }
    preference /= nPrefGenes;
    double temp = (preference - pOpt) / deltap;
    vFemale = exp(-0.5 * temp * temp);

    for (int i = 0; i < nTrtGenes; ++i) {
        trait += trtGenes[i];
    }
    trait /= nTrtGenes;
    temp = (trait - tOpt) / deltat;
    vMale = exp(-0.5 * temp * temp);
}

//

// PROGRAM SECTION

//

void terminateProgram()
// Wait for user input before closing.
{
    std::cout << "Done!" << std::endl;
    char chAnyChar;
    std::cin >> chAnyChar;
    return;
}

bool livingPopulation(int &deadMales, int &deadFemales, const std::vector<Individual> &population)
/*	This function adds up the dead males and dead females in the generation.
    If mortality is such that there are no living males or females, this prints an error and returns false. */
{
    for (int i = 0; i < popSize; ++i) {
        if (i < nFemales) {
            if (population[i].alive == false) {
                ++deadFemales;
            }
        }
        else {
            if (population[i].alive == false) {
                ++deadMales;
            }
        }
    }
    if (deadMales == nMales) {
        std::cerr << "No living males ";
        return false;
    }
    else {
        if (deadFemales == nFemales) {
            std::cerr << "No living females ";
            return false;
        }
        else {
            return true;
        }
    }
}

void arrangeFemales(const int &deadFemales, std::vector<Individual> &population)
// Swaps dead females with alive ones so that they are sorted into two sections.
{
    for (int i = 0; i < nFemales - deadFemales; ++i) {
        if (!population[i].alive) {
            for (int j = nFemales - deadFemales; j < nFemales; ++j) {
                if (population[j].alive) {
                    std::swap(population[i], population[j]);
                    break;
                }
            }
        }
    }
}

void arrangeMales(const int &deadMales, std::vector<Individual> &population)
// Swaps dead males with alive ones so that they are sorted into two sections.
{
    for (int i = nMales; i < popSize - deadMales; ++i) {
        if (!population[i].alive) {
            for (int j = popSize - deadMales; j < popSize; ++j) {
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
    // Set up initial population.
    std::vector<Individual> population(popSize);

    //	Enter generational For loop.
    for (int g = 0; g < gEnd; ++g) {

        // Mortality of males (second half of the population) by trait size.
        for (int i = nMales; i < popSize; ++i) {
            population[i].mortality(traitCost);
        }
        // Rearrange Males and Females in order of living and dead.
        int deadMales = 0;
        int deadFemales = 0;
        if (livingPopulation(deadMales, deadFemales, population) == false) {
            std::cerr << " in generation " << g << '\n';
            terminateProgram();
            exit(1);
        }
        arrangeFemales(deadFemales, population);
        arrangeMales(deadMales, population);

        // Female mate choice.
        // Females select mates and those that fail to reproduce are moved to the end of the vector.
        for (int i = 0; i < nFemales - deadFemales; ++i) {
            population[i].mateSelect(population, deadMales);
        }

        // Rearrange population once more, to ensure that females that didn't reproduce are at the end. important for when females don't sample the entire population.
        deadMales = 0;
        deadFemales = 0;
        if (livingPopulation(deadMales, deadFemales, population) == false) {
            std::cerr << " in generation " << g << '\n';
            terminateProgram();
            exit(1);
        }
        arrangeFemales(deadFemales, population);
        arrangeMales(deadMales, population);

        // Matings
        std::vector<Individual> girls;
        std::vector<Individual> boys;
        for (int i = 0; i < nFemales; ++i) {
            // Set the mother to the current female and the father to the chosen mate of the female.
            int mother = i;
            int father = population[i].getMate();
            // If the mother is dead, choose a random female from the living females and her partner.
            if (population[i].alive == 0) {
                std::uniform_int_distribution<int> pickWoman(0, nFemales - deadFemales - 1);
                mother = pickWoman(generator);
                father = population[mother].getMate();
            }
            if (father < 0) {
                std::cerr << "PROBLEM WITH MATING\n";
            }
            // Produce two offspring and attach one each to the vectors for males and females.
            Individual girl(population[mother], population[father]);
            Individual boy(population[mother], population[father]);
            girls.push_back(girl);
            boys.push_back(boy);
        }
        // Set up the next population in this vector.
        std::vector<Individual> offspring;
        for (int i = 0; i < nFemales; ++i) {
            offspring.push_back(girls[i]);
        }
        for (int i = 0; i < nMales; ++i) {
            offspring.push_back(boys[i]);
        }

        // Set offspring as the current generation.
        for (int i = 0; i < popSize; ++i) {
            population[i] = offspring[i];
        }
    }

    // Terminate Program.
    terminateProgram();
    return 0;
}
