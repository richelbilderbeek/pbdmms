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

void Simulation::run(Parameters& p,
                     std::mt19937& generator) {
    const int pop_size{static_cast<int>(p.get_pop_size())};
    std::vector<Individual> population(pop_size, Individual(p));
    for (int i = 0; i < pop_size; ++i) {
        population[i].init_population(p, generator);
    }
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
        std::vector<Individual> offspring = create_next_gen(p, generator, population);
        population = offspring;
    }
    stats.close();
    histograms.close();
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

std::vector<Individual> Simulation::create_next_gen(Parameters& p,
                                                    std::mt19937& generator,
                                                    std::vector<Individual>& population) {
    std::vector<Individual> offspring;
    offspring.reserve(population.size());
    const int pop_size{static_cast<int>(p.get_pop_size())};
    for (int i = 0; i < pop_size; ++i) {
        const int mother = pick_mother(generator, p, population);
        const int father = pick_father(generator, p, population, mother);
        if (father < 0 || father > pop_size) {
            throw std::invalid_argument(
            "male_viability_function chose an individual outside of the population vector.");
        }
        Individual child(population[mother], population[father], p, generator);
        offspring.push_back(child);
    }
    return offspring;
}

/* This function calculates which individual will become a mother using the
 * cumulative probabilities and a random number. It sets mother to the position
 * the successful female is at in the vector.
 */
int Simulation::pick_mother(std::mt19937& generator,
                            Parameters& p,
                            std::vector<Individual>& population) {
    const int pop_size{static_cast<int>(p.get_pop_size())};
    std::vector<double> female_viab_dist(pop_size);
    crt_female_viability(population, female_viab_dist, p.get_optimal_preference(),
                  p.get_value_of_preference());
    std::uniform_real_distribution<> mother_distribution(0, female_viab_dist[pop_size - 1]);
    const double chosen = mother_distribution(generator);
    if (chosen > female_viab_dist[pop_size - 1])
        throw std::invalid_argument("No mother chosen by pick_mother function.");
    for (int t = 0; t < pop_size; ++t) {
        if (female_viab_dist[t] >= chosen)
            return t;
    }
    assert(!"Should never get here"); //!OCLINT accepted idiom, see Meyers Effective C++
    throw std::logic_error("Should never get here");
}

int Simulation::pick_father(std::mt19937& generator,
                            Parameters& p,
                            std::vector<Individual>& population,
                            const int& mother) {
    const int pop_size{static_cast<int>(p.get_pop_size())};
    std::vector<double> male_viab_dist(pop_size);
    crt_male_viability(population, male_viab_dist, p.get_optimal_trait(), p.get_value_of_trait());
    std::vector<double> attractivity(pop_size);
    for (int i = 0; i < pop_size; ++i) {
        attractivity[i] = male_viab_dist[i] *
                exp(population[mother].get_preference() *
                    population[i].get_trait() *
                    population[i].get_quality());
    }
    std::uniform_real_distribution<> father_distribution(0, attractivity[pop_size - 1]);
    const double chosen = father_distribution(generator);
    if (chosen > attractivity[pop_size - 1])
        throw std::invalid_argument("No mother chosen by pick_mother function.");
    for (int t = 0; t < pop_size; ++t) {
        if (attractivity[t] >= chosen)
            return t;
    }
    assert(!"Should never get here"); //!OCLINT accepted idiom, see Meyers Effective C++
    throw std::logic_error("Should never get here");
}

/* This function calculates the cumulative viability for each individual in the vector
 * so that when a random number is drawn up to the cumulative viability, an individual
 * has a greater or lesser chance of being chosen based on its individual viability.
 */
void Simulation::crt_female_viability(std::vector<Individual>& population,
                                      std::vector<double>& viab_dist,
                                      const double& optimal_preference,
                                      const double& value_of_preference) {
    const int pop_size{static_cast<int>(population.size())};
    double temp = (population[0].get_preference() - optimal_preference) / value_of_preference;
    viab_dist[0] = exp(-0.5 * temp * temp);
    assert(viab_dist[0] >= 0);
    for (int i = 1; i < pop_size; ++i) {
        temp = (population[i].get_preference() - optimal_preference) / value_of_preference;
        viab_dist[i] = viab_dist[i-1] + exp(-0.5 * temp * temp);
        assert(viab_dist[i] >= viab_dist[i-1]);
    }
}

void Simulation::crt_male_viability(std::vector<Individual>& population,
                                    std::vector<double>& viab_dist,
                                    const double& optimal_trait,
                                    const double& value_of_trait) {
         const int pop_size{static_cast<int>(population.size())};
         double temp = (population[0].get_trait() - optimal_trait) / value_of_trait;
         viab_dist[0] = exp(-0.5 * temp * temp);
         assert(viab_dist[0] >= 0);
         for (int i = 1; i < pop_size; ++i) {
             temp = (population[i].get_trait() - optimal_trait) / value_of_trait;
             viab_dist[i] = viab_dist[i-1] + exp(-0.5 * temp * temp);
             assert(viab_dist[i] >= viab_dist[i-1]);
         }
     }
