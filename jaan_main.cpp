/*	jaan_main.cpp
    Written by James Andrews on 17-10-2016
    A simulation of a population with sexual selection.
    Based on Cichlids in Lake Victoria.

    ASSUMPTIONS
    Haploid population
    Multiple loci with weak increase on ornament and preference.
    Sequential, fixed interval sampling in mate selection.
    Mate selection prefers bigger ornaments.
    Fixed, discrete generations.

    From here:
     - Two different environments
     - Two way selection of ornament? Or should the ornament be the same and perception is changed?
     - Environment dependent mortality
     - Environment dependent selection criteria
     - Movement between environments and costs for movement
     - Environment dependent ecological selection
*/

#include <iostream>
#include <fstream>
#include <vector>
#include "jaan_random.h"
#include "jaan_utils.h"

const int gEnd			= 5000;												// Number of Generations to run through.
const int popSize		= 100;												// Population size.
const int nMales		= popSize / 2;										// How many individuals in a vector are treated as males.
const int nFemales		= popSize - nMales;									// How many individuals in a vector are treated as females.
const int nPrefGenes	= 10;												// Number of genes for Preference.
const int nOrnGenes		= 10;												// Number of genes for Ornament.
const double mu			= 1.0e-4;											// Mutation rate.

//

// CLASS SECTION

//

class Individual
// Individual class declaration.
{
public:
    Individual();
    Individual(const Individual&, const Individual&);
    //Individual(const Individual&);
    //Individual& operator=(const Individual&);
    void mateSelect(const std::vector<Individual>&, const int);
    void mortality(const double ornamentCost);
    double getPref();
    double getOrn();
    int getMate();
    bool alive;
private:
    void mutate();
    void develop();
    std::vector<double> prefGenes;
    std::vector<double> ornGenes;
    double preference;
    double ornament;
    int mate;
};

// PUBLIC INDIVIDUAL CLASS FUNCTIONS

// INDIVIDUAL CLASS CONSTRUCTORS
Individual::Individual() :
// Initialisation constructor.
alive(true),
prefGenes(nPrefGenes),
ornGenes(nOrnGenes),
preference(0.0),
ornament(0.0),
mate(-1)
{
    for (int i = 0; i < nOrnGenes; ++i) {									// Randomly set each gene to 0 or 1.
        if (rnd::uniform() < 0.2) {
            ornGenes[i] = 0;
        }
        else {
            ornGenes[i] = 1;
        }
    }
    for (int i = 0; i < nPrefGenes; ++i) {									// Randomly set each Preference gene to 0 or 1.
        if (rnd::uniform() < 0.8) {
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
ornGenes(nOrnGenes),
preference(0.0),
ornament(0.0),
mate(-1)
{
    for (int i = 0; i < nOrnGenes; ++i) {									// Inherit mother and father genes randomly 50:50.
        if (rnd::uniform() < 0.5) {
            ornGenes[i] = mother.ornGenes[i];
        }
        else {
            ornGenes[i] = father.ornGenes[i];
        }
    }
    for (int i = 0; i < nPrefGenes; ++i) {									// Inherit mother and father preference genes randomly 50:50.
        if (rnd::uniform() < 0.5) {
            prefGenes[i] = mother.prefGenes[i];
        }
        else {
            prefGenes[i] = father.prefGenes[i];
        }
    }
    mutate();
    develop();
}

/*
Individual::Individual(const Individual& ind) :
// Copy constructor
alive(ind.alive),
prefGenes(ind.prefGenes),
ornGenes(ind.ornGenes),
preference(ind.preference),
ornament(ind.ornament),
mate(ind.mate)
{
    ornGenes = ind.ornGenes;
        for (int i = 0; i < nPrefGenes; ++i) {
        prefGenes[i] = ind.prefGenes[i];
    }
}
*/
/*
Individual& Individual::operator=(const Individual &ind)
// Assignment operator
{
    ornGenes	= ind.ornGenes;
    prefGenes	= ind.prefGenes;
    preference	= ind.preference;
    ornament	= ind.ornament;
    alive		= ind.alive;
    mate		= ind.mate;
    return *this;
}
*/

// CLASS FUNCTIONS
void Individual::mateSelect(const std::vector<Individual>& population, const int deadMales)
/*	Function for females to find a partner.
    Takes a vector of all individuals and selects randomly from the second half of the vector.
    Function returns -1 if no suitable mate is found, otherwise returns the position of the male. */
{
    for (double t = 0.0, d = nMales; t < d; ++t) {							// Sample the males at random.
        int focal = nMales + rnd::integer(nMales - deadMales);

        if (population[focal].ornament >= preference) {
            mate = focal;													// If the male is good, return his position in vector.
        }
    }
    if (mate == -1) {
        alive = false;
    }
}

void Individual::mortality(const double ornamentCost)
// Causes mortality for males based on ornament cost.
{
    if (ornament * ornamentCost > rnd::uniform()) {
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

double Individual::getOrn()
{
    return ornament;
}

//PRIVATE INDIVIDUAL CLASS FUNCTIONS
void Individual::mutate()
// Give each gene a chance of flipping.
{
    for (int i = 0; i < nOrnGenes; ++i) {
        if (rnd::uniform() < mu)  {
            if (ornGenes[i] == 1) {
                ornGenes[i] = 0;
            }
            else ornGenes[i] = 1;
        }
    }
    for (int i = 0; i < nPrefGenes; ++i) {
        if (rnd::uniform() < mu) {
            if (prefGenes[i] == 1) {
                prefGenes[i] = 0;
            }
            else prefGenes[i] = 1;
        }
    }
}

void Individual::develop()
/*	Calculate preference from genes 1 to nPreferenceGenes.
    Calculate ornament from genes nPreferenceGenes to nGenes.*/
{
    for (int i = 0; i < nPrefGenes; ++i) {
        preference += prefGenes[i];
    }
    preference /= nPrefGenes;

    for (int i = 0; i < nOrnGenes; ++i) {
        ornament += ornGenes[i];
    }
    ornament /= nOrnGenes;
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

void swap(Individual &A, Individual &B)
// swaps Individual A with Individual B.
{
    Individual C = A;
    A = B;
    B = C;
    return;
}

void arrangeFemales(const int &deadFemales, std::vector<Individual> &population)
// Swaps dead females with alive ones so that they are sorted into two sections.
{
    for (int i = 0; i < nFemales - deadFemales; ++i) {
        if (!population[i].alive) {
            for (int j = nFemales - deadFemales; j < nFemales; ++j) {
                if (population[j].alive) {
                    swap(population[i], population[j]);
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
                    swap(population[i], population[j]);
                    break;
                }
            }
        }
    }
}

int main()
{
    rnd::set_seed();
    // Set up initial population.
    std::vector<Individual> population(popSize);

    std::ofstream output("data.csv");

    // Enter ornamentCost for loop.
    for (double ornamentCost = 0.0; ornamentCost < 1.1; ornamentCost += 0.1) {
        // Enter repetition for loop.
        for (int r = 0; r < 10; ++r) {
            //	Enter generational For loop.
            std::cout << "Cost\tR\tG\tPref\tOrn\n";
            output << "Cost\tR\tG\tPref\tOrn\n";
            for (int g = 0; g < gEnd; ++g) {

                // Mortality of males (second half of the population) by ornament size.
                for (int i = nMales; i < popSize; ++i) {
                    population[i].mortality(ornamentCost);
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
                        mother = rnd::integer(nFemales - deadFemales);
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

                // Calculate average ornament and preference in the population.
                double avPref = 0.0;
                for (int i = 0; i < nFemales; ++i) {
                    avPref += population[i].getPref();
                }
                avPref /= nFemales;											// Calculate the average preference of females in this generation.
                double avOrn = 0.0;
                for (int i = nMales; i < popSize; ++i) {
                    avOrn += population[i].getOrn();
                }
                avOrn /= nMales;											// Calculate the average ornamentation of males in this generation.
                std::cout << ornamentCost << '\t' << r << '\t' << g << '\t' << avPref << '\t' << avOrn << '\n';
                output << ornamentCost << '\t' << r << '\t' << g << '\t' << avPref << '\t' << avOrn << '\n';

                // Set offspring as the current generation.
                for (int i = 0; i < popSize; ++i) {
                    population[i] = offspring[i];
                }
            }
            std::cout << "\n\n";
            output << "\n\n";
        }
        std::cout << "\n\n";
        output << "\n\n";
    }

    // Terminate Program.
    output.close();
    terminateProgram();
    return 0;
}
