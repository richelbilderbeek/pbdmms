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
    statistics(p, population, output);
    histogram(p, population, output);
}

void Simulation::statistics(Parameters& p,
                            std::vector<Individual>& population,
                            std::ofstream& output) {
    // Calculate the mean, variance of pref and trt and covariance of pref and trt.
    double sum_pref = 0;
    double sum_trt = 0;
    double sum_trt_and_pref = 0;
    double sum_pref_squared = 0;
    double sum_trt_squared = 0;
    const int pop_size{static_cast<int>(p.get_pop_size())};
    for (int i = 0; i < pop_size; ++i) {
        const int preference{static_cast<int>(population[i].get_preference())};
        const int trait{static_cast<int>(population[i].get_trait())};
        sum_pref += preference;
        sum_trt += trait;
        sum_trt_and_pref += (preference * trait);
        sum_pref_squared += (preference * preference);
        sum_trt_squared += (trait * trait);
    }
    const double mean_pref = sum_pref / pop_size;
    const double mean_trt = sum_trt / pop_size;
    const double covariance = (sum_trt_and_pref - (sum_trt_and_pref / pop_size)) / pop_size;
    const double pref_variance = (sum_pref_squared - (sum_pref * sum_pref) / pop_size) / pop_size;
    const double trt_variance = (sum_trt_squared - (sum_trt * sum_trt) / pop_size) / pop_size;
    output << "mean_pref " << mean_pref <<
              " mean_trt " << mean_trt <<
              " covariance " << covariance <<
              " pref_variance " << pref_variance <<
              " trt_variance " << trt_variance << std::endl;
}

void Simulation::histogram(Parameters& p,
                           std::vector<Individual>& population,
                           std::ofstream& output) {
    /* Create two histograms, one of preferences in the population and one of male traits.
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
    output_histogram(pref_hist, trt_hist, output);
}

void Simulation::output_histogram(const std::vector<double>& pref_hist,
                                  const std::vector<double>& trt_hist,
                                  std::ofstream& output) {
    output << "Preference Histogram" << std::endl;
    const int pref_hist_size{static_cast<int>(pref_hist.size())};
    for (int i = 0; i < pref_hist_size; ++i) {
        output << pref_hist[i] << " ";
    }
    output << "\t Trait Histogram" << std::endl;
    const int trt_hist_size{static_cast<int>(trt_hist.size())};
    for (int i = 0; i < trt_hist_size; ++i) {
        output << trt_hist[i] << " ";
    }
    output << std::endl;
}

void Simulation::run(Parameters& p,
                     std::mt19937& generator) {
    std::vector<Individual> population(p.get_pop_size(), Individual(p, generator));
    std::ofstream output("jaan_output.csv");
    for (int g = 0; g < p.get_max_generations(); ++g) {
        double cumulative_viab = female_viability(p, generator, population);
        std::uniform_real_distribution<> pick_mother(0, cumulative_viab);
        std::vector<Individual> offspring = create_next_gen(p,
                                                            generator,
                                                            population,
                                                            pick_mother);
        if (g == 0)
            output << "generation " << g << std::endl;
            data_collection(p, population, output);
        if ((g % 100) == 0)
            output << "generation " << g << std::endl;
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
    const int pop_size{static_cast<int>(p.get_pop_size())};
    for (int i = 0; i < pop_size; ++i) {
        population[i].mate_select(population, p, generator);
        const double female_viability{static_cast<double>(population[i].get_female_viability())};
        cumulative_viab += female_viability;
        population[i].set_female_viability(female_viability + cumulative_viab);
    }
    return cumulative_viab;
}

/* This function calculates which individual will become a mother using the
 * cumulative probabilities and a random number. It sets mother to the position
 * the successful female is at in the vector.
 */
int Simulation::mother_choosing(Parameters& p,
                                std::vector<Individual>& population,
                                const double chosen) {
    const int pop_size{static_cast<int>(p.get_pop_size())};
    if (chosen > population[pop_size - 1].get_female_viability())
        throw std::invalid_argument("No mother chosen by mother_choosing function.");
    for (int t = 0; t < pop_size; ++t) {
        const double female_viability{static_cast<double>(population[t].get_female_viability())};
        if (t != 0)
            assert(female_viability > population[t-1].get_female_viability());
        if (female_viability >= chosen)
            return t;
    }
    assert(!"Should never get here"); //!OCLINT accepted idiom, see Meyers Effective C++
    throw std::logic_error("Should never get here");
}

std::vector<Individual> Simulation::create_next_gen(Parameters& p,
                                                    std::mt19937& generator,
                                                    std::vector<Individual>& population,
                                                    std::uniform_real_distribution<> pick_mother) {
    std::vector<Individual> offspring;
    offspring.reserve(population.size());
    const int pop_size{static_cast<int>(p.get_pop_size())};
    for (int i = 0; i < pop_size; ++i) {
        const double chosen = pick_mother(generator);
        const int mother = mother_choosing(p, population, chosen);
        const int father = population[mother].get_mate();
        if (father < 0 || father > pop_size) {
            throw std::invalid_argument(
                        "mate_select chose an individual outside of the population vector.");
        }
        Individual child(population[mother], population[father], p, generator);
        offspring.push_back(child);
    }
    return offspring;
}
