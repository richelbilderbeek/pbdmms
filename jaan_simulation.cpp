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

// Calculate the mean and variance of pref and trt and covariance of pref and trt.
void Simulation::statistics(Parameters& p,
                            std::vector<Individual>& population,
                            std::ofstream& stats) {
    double sum_pref = 0;
    double sum_trt = 0;
    double sum_trt_and_pref = 0;
    double sum_pref_squared = 0;
    double sum_trt_squared = 0;
    const int pop_size{static_cast<int>(p.get_pop_size())};
    for (int i = 0; i < pop_size; ++i) {
        const double preference{static_cast<double>(population[i].get_preference())};
        const double trait{static_cast<double>(population[i].get_trait())};
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
    std::cout << "mean_pref " << mean_pref <<
                 " pref_variance " << pref_variance <<
                 " mean_trt " << mean_trt <<
                 " trt_variance " << trt_variance <<
                 " covariance " << covariance << std::endl;
    stats << mean_pref << ','
          << pref_variance << ','
          << mean_trt << ','
          << trt_variance << ','
          << covariance << std::endl;
}


/* Create two histograms, one of preferences in the population and one of male traits.
 * Possible values for the histogram run from all -1 to all +1 so the size is the difference
 * between the two, i.e. the all -1, all +1 plus the all 0 state.
 *
 * Except this isn't true, since any +1 turning to -1 changes it by 2 so they can't be any value.
 */
void Simulation::histogram(Parameters& p,
                           std::vector<Individual>& population,
                           std::ofstream& histograms) {
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
    output_histogram(pref_hist, trt_hist, histograms);
}

void Simulation::output_histogram(const std::vector<double>& pref_hist,
                                  const std::vector<double>& trt_hist,
                                  std::ofstream& histograms) {
    std::cout << "Preference_Histogram ";
    histograms << "Preference_Histogram,";
    const int pref_hist_size{static_cast<int>(pref_hist.size())};
    for (int h = (-0.5 * (pref_hist_size - 1)); h < (0.5 * (pref_hist_size - 1)); ++h) {
        histograms << h << ',';
    }
    histograms << std::endl;
    for (int i = 0; i < pref_hist_size; ++i) {
        std::cout << pref_hist[i] << " ";
        histograms << pref_hist[i] << ",";
    }
    std::cout << "\nTrait_Histogram ";
    histograms << "\nTrait_Histogram,";
    const int trt_hist_size{static_cast<int>(trt_hist.size())};
    for (int h = (-0.5 * (trt_hist_size - 1)); h < (0.5 * (trt_hist_size - 1)); ++h) {
        histograms << h << ',';
    }
    for (int i = 0; i < trt_hist_size; ++i) {
        std::cout << trt_hist[i] << " ";
        histograms << trt_hist[i] << ",";
    }
    std::cout << std::endl;
    histograms << std::endl;
}

void Simulation::run(Parameters& p,
                     std::mt19937& generator) {
    std::vector<Individual> population(p.get_pop_size(), Individual(p, generator));
    std::ofstream stats("jaan_stats.csv");
    std::ofstream histograms("jaan_histograms.csv");
    stats << "generation,mean_pref,pref_variance,mean_trt,trt_variance,covariance" << std::endl;
    for (int g = 0; g < p.get_max_generations(); ++g) {
        std::cout << "generation " << g << std::endl;
        if ((g % 100) == 0) {
            stats << g << ',';
            histograms << "generation," << g << std::endl;
            statistics(p, population, stats);
            histogram(p, population, histograms);
        }
        double cumulative_viab = female_viability(p, population);
        std::uniform_real_distribution<> mother_distribution(0, cumulative_viab);
        std::vector<Individual> offspring = create_next_gen(p,
                                                            generator,
                                                            population,
                                                            mother_distribution);
        population = offspring;
    }
    stats.close();
    histograms.close();
}

/* This function calculates the cumulative viability for each individual in the vector
 * so that when a random number is drawn up to the cumulative viability, an individual
 * has a greater or lesser chance of being chosen based on its individual viability.
 */
double Simulation::female_viability(Parameters& p,
                                    std::vector<Individual>& population) {
    double cumulative_viab = 0.0;
    const int pop_size{static_cast<int>(p.get_pop_size())};
    for (int i = 0; i < pop_size; ++i) {
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
                        std::uniform_real_distribution<> mother_distribution) {
    std::vector<Individual> offspring;
    offspring.reserve(population.size());
    const int pop_size{static_cast<int>(p.get_pop_size())};
    for (int i = 0; i < pop_size; ++i) {
        const double chosen = mother_distribution(generator);
        const int mother = mother_choosing(p, population, chosen);
        const int father = population[mother].male_viability_function(population, p, generator);
        if (father < 0 || father > pop_size) {
            throw std::invalid_argument(
            "male_viability_function chose an individual outside of the population vector.");
        }
        Individual child(population[mother], population[father], p, generator);
        offspring.push_back(child);
    }
    return offspring;
}
