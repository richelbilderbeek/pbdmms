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

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <cmath>
#include "jaan_individual.h"
#include "jaan_parameters.h"

/* This function calculates the cumulative viability for each individual in the vector
 * so that when a random number is drawn up to the cumulative viability, an individual
 * has a greater or lesser chance of being chosen based on its individual viability.
 */
double female_choice_male_viability(Parameters& p,
                                    std::mt19937& generator,
                                    std::vector<Individual>& population) {
    double cumulative_viab = 0.0;
    for (int i = 0; i < p.get_popSize(); ++i) {
        population[i].mateSelect(population, p, generator);
        cumulative_viab += population[i].vFemale;
        population[i].vFcum += cumulative_viab;
    }
    return cumulative_viab;
}

/* This function calculates which individual will become a mother using the
 * cumulative probabilities and a random number. It sets mother to the position
 * the successful female is at in the vector.
 */
int mother_choosing(Parameters& p,
                     std::vector<Individual>& population,
                     double chosen) {
    int mother = -1;
    for (int t = 0; t < p.get_popSize(); ++t) {
        if (population[0].vFcum < chosen) {
            mother = 0;
        }
        else if ((t == p.get_popSize() - 1) && (population[t].vFcum < chosen)) {
            mother = p.get_popSize();
        }
        else if ((population[t].vFcum > chosen) && (population[t-1].vFcum < chosen)) {
            mother = t + 1;
        }
    }
    if (mother < 0 || mother > p.get_popSize()) {
        throw std::invalid_argument("No father accepted by female_viability function.");
    } else {
        return mother;
    }
}

std::vector<Individual> create_next_generation(Parameters& p,
                                               std::mt19937& generator,
                                               std::vector<Individual>& population,
                                               double& chosen
                                               )
{
    std::vector<Individual> offspring;
    offspring.reserve(population.size());
    for (int i = 0; i < p.get_popSize(); ++i) {
        int mother = mother_choosing(p, population, chosen);
        int father = population[mother].getMate();
        if (father < 0 || father > p.get_popSize()) {
            throw std::invalid_argument(
                        "mateSelect chose an individual outside of the population vector.");
        }
        Individual child(population[mother], population[father], p, generator);
        offspring.push_back(child);
    }
    return offspring;
}

int main() {
    Parameters p;
    std::mt19937 generator;
    std::vector<Individual> population(p.get_popSize(), Individual(p, generator));

    for (int g = 0; g < p.get_gEnd(); ++g) {
        double cumulative_viab = female_choice_male_viability(p, generator, population);
        std::uniform_real_distribution<double> pickMother(0, cumulative_viab);
        double chosen = pickMother(generator);
        std::vector<Individual> offspring = create_next_generation(p,
                                                                   generator,
                                                                   population,
                                                                   chosen);
        population = offspring;
    }

    return 0;
}
