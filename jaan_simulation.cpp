#include <iostream>
#include <vector>
#include <random>
#include <stdexcept>
#include "jaan_simulation.h"
#include "jaan_parameters.h"

Simulation::Simulation() {
}

void Simulation::data_collection(Parameters& p,
                                 std::vector<Individual>& population) {
    std::vector<int> pref_hist((p.get_nPrefGenes() * 2) + 1);
    /* For each individual, test whether its preference should fit into
     */
    for (int i = 0; i < p.get_popSize(); ++i) {
        for (int h = 0; h < (1 + (2 * p.get_nPrefGenes())); h++) {
            if (population[i].get_Pref() < h + 1 - p.get_nPrefGenes()) {
                pref_hist[h]++;
                break;
            }
        }
    }
}

void Simulation::run(Parameters& p,
                     std::mt19937& generator) {
    std::vector<Individual> population(p.get_popSize(), Individual(p, generator));
    for (int g = 0; g < p.get_gEnd(); ++g) {
        double cumulative_viab = female_viability(p, generator, population);
        std::uniform_real_distribution<> pickMother(0, cumulative_viab);
        std::vector<Individual> offspring = create_next_gen(p,
                                                            generator,
                                                            population,
                                                            pickMother);
        if (g == 0) {
            data_collection(p, population);
        }
        else if ((p.get_gEnd() % g) == 0) {
            data_collection(p, population);
        }
        population = offspring;
        std::cout << g << std::endl;
    }
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
    int mother = -1;
    for (int t = 0; t < p.get_popSize(); ++t) {
        if (population[0].get_vFemale() > chosen) {
            mother = 0;
            return mother;
        }
        else if ((t == p.get_popSize() - 1) && (population[t].get_vFemale() < chosen)) {
            mother = p.get_popSize();
            return mother;
        }
        else if ((population[t].get_vFemale() > chosen) &&
                 (population[t-1].get_vFemale() < chosen)) {
            mother = t;
            return mother;
        }
    }
    if (mother < 0 || mother > p.get_popSize()) {
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
