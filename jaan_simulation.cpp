#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <stdexcept>
#include "jaan_simulation.h"
#include "jaan_parameters.h"

Simulation::Simulation() {
}


/*
 *
 * THIS FUNCTION NEEDS CHOPPING DOWN INTO SMALLER CHUNKS.
 *
 * THIS FUNCTION NEEDS UPDATING WITH THE CORRECT CUMVAR AND VAR FUNCTIONS LIKE SANDER SHOWED ME.
 *
 *
 */
void Simulation::data_collection(Parameters& p,
                                 std::vector<Individual>& population,
                                 std::ofstream& output) {
     /* Create two histograms, one of preferences in the population and one of ornaments.
     * Possible values for the histogram run from all -1 to all +1 so the size is the difference
     * between the two, i.e. the all -1, all +1 plus the all 0 state.
     */
    std::vector<double> pref_hist((p.get_nPrefGenes() * 2) + 1);
    std::vector<double> trt_hist((p.get_nTrtGenes() * 2) + 1);
    double sumPref = 0;
    double sumTrt = 0;
    const int popSize{static_cast<int>(p.get_popSize())};
    const int nPrefGenes{static_cast<int>(p.get_nPrefGenes())};
    const int nTrtGenes{static_cast<int>(p.get_nTrtGenes())};
    for (int i = 0; i != popSize; ++i) {
        sumPref += population[i].get_Pref();
        sumTrt += population[i].get_Trt();
        for (int h = 0; h < (1 + (2 * nPrefGenes)); ++h) {
            if (population[i].get_Pref() < h + 1 - nPrefGenes) {
                ++pref_hist[h];
                break;
            }
        }
        for (int h = 0; h < (1 + (2 * nTrtGenes)); ++h) {
            if (population[i].get_Trt() < h + 1 - nTrtGenes) {
                ++trt_hist[h];
                break;
            }
        }
    }

    // Calculate the mean, variance and covariance of the population
    double meanPref = sumPref / popSize;
    double meanTrt = sumTrt / popSize;
    double varPref = 0;
    double varTrt = 0;
    double covar = 0;
    for (int i = 0; i != popSize; ++i) {
        double diff1 = population[i].get_Pref() - meanPref;
        varPref += (diff1 * diff1);
        double diff2 = population[i].get_Trt() - meanTrt;
        varTrt += (diff2 * diff2);
        covar += (diff1 * diff2);
    }
    varPref /= popSize;
    varTrt /= popSize;
    covar /= popSize;

    // Add in section for retrieving ALL the stats.
    output << varPref << std::endl;
}

void Simulation::run(Parameters& p,
                     std::mt19937& generator) {
    std::vector<Individual> population(p.get_popSize(), Individual(p, generator));
    std::ofstream output("output.csv");
    for (int g = 0; g < p.get_gEnd(); ++g) {
        double cumulative_viab = female_viability(p, generator, population);
        std::uniform_real_distribution<> pickMother(0, cumulative_viab);
        std::vector<Individual> offspring = create_next_gen(p,
                                                            generator,
                                                            population,
                                                            pickMother);
        if (g == 0) {
            data_collection(p, population, output);
        }
        else if ((g % 5) == 0) {
            data_collection(p, population, output);
        }
        population = offspring;
        std::cout << g << std::endl;
    }
    output.close();
}

/* This function calculates the cumulative viability for each individual in the vector
 * so that when a random number is drawn up to the cumulative viability, an individual
 * has a greater or lesser chance of being chosen based on its individual viability.
 */
double Simulation::female_viability(Parameters& p,
                                    std::mt19937& generator,
                                    std::vector<Individual>& population) {
    double cumulative_viab = 0.0;
    for (int i = 0; i < p.get_popSize(); ++i) {
        population[i].mateSelect(population, p, generator);
        cumulative_viab += population[i].get_vFemale();
        population[i].set_vFemale(population[i].get_vFemale() + cumulative_viab);
    }
    return cumulative_viab;
}

/* This function calculates which individual will become a mother using the
 * cumulative probabilities and a random number. It sets mother to the position
 * the successful female is at in the vector.
 */
int Simulation::mother_choosing(Parameters& p,
                                std::vector<Individual>& population,
                                double chosen) {
    for (int t = 0; t < p.get_popSize(); ++t) {
        if (t != 0)
            assert(population[t].get_vFemale() > population[t-1].get_vFemale());
        if ((population[t].get_vFemale() > chosen))
            return t;
    }
    if (chosen > population[p.get_popSize() - 1].get_vFemale()) {
        throw std::invalid_argument("No mother chosen by mother_choosing function.");
    }
}

std::vector<Individual> Simulation::create_next_gen(Parameters& p,
                                                    std::mt19937& generator,
                                                    std::vector<Individual>& population,
                                                    std::uniform_real_distribution<> pickMother) {
    std::vector<Individual> offspring;
    offspring.reserve(population.size());
    for (int i = 0; i < p.get_popSize(); ++i) {
        double chosen = pickMother(generator);
        int mother = mother_choosing(p, population, chosen);
        int father = population[mother].get_Mate();
        if (father < 0 || father > p.get_popSize()) {
            throw std::invalid_argument(
                        "mateSelect chose an individual outside of the population vector.");
        }
        Individual child(population[mother], population[father], p, generator);
        offspring.push_back(child);
    }
    return offspring;
}
