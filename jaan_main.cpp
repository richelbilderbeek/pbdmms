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

    RIGHT NOW I'M CHANGING MATESELECT TO SCORE THE POPULATION ACCORDING TO PREFERENCE AND THEN
    IT OUGHT TO PICK MUCH LIKE THE PICKMOTHER SECTION WHICH SHOULD BE A FATHER.
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
#include "jaan_individual.h"
#include "jaan_parameters.h"

int main()
{
    Parameters p;
    std::mt19937 generator;

    // Random number generator engine for putting into the uniform distribution functions.
    // Set up initial population.
    std::vector<Individual> population(p.get_popSize(), Individual(p, generator));

    //	Enter generational For loop.
    for (int g = 0; g < p.get_gEnd(); ++g) {
        // Calculate male viability.

        // Female mate choice and male viability.
        double cum_viab = 0.0;
        for (int i = 0; i < p.get_popSize(); ++i) {
            population[i].mateSelect(population, p, generator);
            cum_viab += population[i].vFemale;    // Cumulative probabilty variable.
            population[i].vFcum += cum_viab;      // Cumulative probability up to individual i.
        }
        // Create a uniform distribution up to the size of the cumulative probability.
        std::uniform_real_distribution<double> pickMother(0, cum_viab);

        // Matings
        std::vector<Individual> offspring;
        offspring.reserve(population.size());
        for (int i = 0; i < p.get_popSize(); ++i) {
            // Choose a random number from the probability distribution.
            double chosen = pickMother(generator);
            std::cout << "chosen is " << chosen << "\n";
            int mother = -1;
            for (int t = 0; t < p.get_popSize(); ++t) {
                std::cout << "vFcum = " << population[t].vFcum << "\n";
                if (population[0].vFcum < chosen) {
                    mother = 0;
                }
                else if ((t == p.get_popSize() - 1) & (population[t].vFcum < chosen)) {
                    mother = p.get_popSize();
                }
                else if ((population[t].vFcum > chosen) & (population[t-1].vFcum < chosen)) {
                    mother = t + 1;
                }
            }
            if (mother == -1) {
                std::cerr  << "Problem with setting mother.";
                exit(1);
            }
            // Set the mother to the current female and the father to the 'mate' of the female.
            int father = population[mother].getMate();
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

    return 0;
}
