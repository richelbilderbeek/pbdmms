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

void Simulation::data_collection(Parameters& p,
                                 std::vector<Individual>& population,
                                 std::ofstream& output) {
    histogram(p, population, output);
    statistics(p, population, output);
}

void Simulation::statistics(Parameters& p,
                            std::vector<Individual>& population,
                            std::ofstream& output) {
    // Calculate the mean, variance and covariance of the population
    double sum_pref = 0;
    double sum_trt = 0;
    double sum_trt_and_pref = 0;
    double sum_pref_squared = 0;
    double sum_trt_squared = 0;
    const int pop_size{static_cast<int>(p.get_pop_size())};
    for (int i = 0; i < pop_size; ++i) {
        sum_pref += population[i].get_preference();
        sum_trt += population[i].get_trait();
        sum_trt_and_pref += (population[i].get_preference() * population[i].get_trait());
        sum_pref_squared += (population[i].get_preference() * population[i].get_preference());
        sum_trt_squared += (population[i].get_trait() * population[i].get_trait());
    }
    double covariance = (sum_trt_and_pref - (sum_trt_and_pref / pop_size)) / pop_size;
    double pref_variance = (sum_pref_squared - (sum_pref * sum_pref) / pop_size) / pop_size;
    double trt_variance = (sum_trt_squared - (sum_trt * sum_trt) / pop_size) / pop_size;
    output << covariance << " " << pref_variance << " " << trt_variance << std::endl;
}

void Simulation::histogram(Parameters& p,
                           std::vector<Individual>& population,
                           std::ofstream& output) {
    /* Create two histograms, one of preferences in the population and one of ornaments.
     * Possible values for the histogram run from all -1 to all +1 so the size is the difference
     * between the two, i.e. the all -1, all +1 plus the all 0 state.
     */
    std::vector<double> pref_hist((p.get_n_pref_genes() * 2) + 1);
    std::vector<double> trt_hist((p.get_n_trt_genes() * 2) + 1);
    const int pop_size{static_cast<int>(p.get_pop_size())};
    const int n_pref_genes{static_cast<int>(p.get_n_pref_genes())};
    const int n_trt_genes{static_cast<int>(p.get_n_trt_genes())};
    for (int i = 0; i < pop_size; ++i) {
        for (int h = 0; h < (1 + (2 * n_pref_genes)); ++h) {
            if (population[i].get_preference() < h + 1 - n_pref_genes) {
                ++pref_hist[h];
                break;
            }
        }
        for (int h = 0; h < (1 + (2 * n_trt_genes)); ++h) {
            if (population[i].get_trait() < h + 1 - n_trt_genes) {
                ++trt_hist[h];
                break;
            }
        }
    }
    output_histogram(p, pref_hist, trt_hist, output);
}

void Simulation::output_histogram(Parameters& p,
                                  std::vector<double>& pref_hist,
                                  std::vector<double>& trt_hist,
                                  std::ofstream& output) {
    const int pop_size{static_cast<int>(p.get_pop_size())};
    for (int i = 0; i < pop_size; ++i) {
        output << pref_hist[i] << " " << trt_hist[i] << std::endl;
    }
}

void Simulation::run(Parameters& p,
                     std::mt19937& generator) {
    std::vector<Individual> population(p.get_pop_size(), Individual(p, generator));
    std::ofstream output("output.csv");
    for (int g = 0; g < p.get_max_generations(); ++g) {
        double cumulative_viab = female_viability(p, generator, population);
        std::uniform_real_distribution<> pick_mother(0, cumulative_viab);
        std::vector<Individual> offspring = create_next_gen(p,
                                                            generator,
                                                            population,
                                                            pick_mother);
        if (g == 0)
            data_collection(p, population, output);
        else if ((g % 5) == 0)
            data_collection(p, population, output);
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
    for (int i = 0; i < p.get_pop_size(); ++i) {
        population[i].mate_select(population, p, generator);
        cumulative_viab += population[i].get_female_viability();
        population[i].set_female_viability(population[i].get_female_viability() + cumulative_viab);
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
    for (int t = 0; t < p.get_pop_size(); ++t) {
        if (t != 0)
            assert(population[t].get_female_viability() > population[t-1].get_female_viability());
        if ((population[t].get_female_viability() > chosen))
            return t;
    }
    if (chosen > population[p.get_pop_size() - 1].get_female_viability())
        throw std::invalid_argument("No mother chosen by mother_choosing function.");
}

std::vector<Individual> Simulation::create_next_gen(Parameters& p,
                                                    std::mt19937& generator,
                                                    std::vector<Individual>& population,
                                                    std::uniform_real_distribution<> pick_mother) {
    std::vector<Individual> offspring;
    offspring.reserve(population.size());
    for (int i = 0; i < p.get_pop_size(); ++i) {
        double chosen = pick_mother(generator);
        int mother = mother_choosing(p, population, chosen);
        int father = population[mother].get_mate();
        if (father < 0 || father > p.get_pop_size()) {
            throw std::invalid_argument(
                        "mate_select chose an individual outside of the population vector.");
        }
        Individual child(population[mother], population[father], p, generator);
        offspring.push_back(child);
    }
    return offspring;
}
