#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include "jaan_simulation.h"
#include "jaan_parameters.h"

Simulation::Simulation() {
}

void Simulation::run(const Parameters& p,
                     std::mt19937& generator) {
    const int pop_size{static_cast<int>(p.get_pop_size())};
    std::vector<Individual> population(pop_size, Individual(p));
    for (int i = 0; i < pop_size; ++i) {
        population[i].init_population(p, generator);
    }
    std::ofstream stats("jaan_stats.csv");
    std::ofstream histograms("jaan_histograms.csv");
    stats << "generation,mean_pref,mean_trt,pref_variance,trt_variance,covariance" << std::endl;
    for (int g = 0; g < p.get_max_generations(); ++g) {
        std::cout << "generation " << g << std::endl;
        if ((g % 100) == 0) {
            stats << g << ',';
            histograms << "generation," << g << std::endl;
            statistics(p, population, stats);
            histogram(p, population, histograms);
        }
        population = create_next_gen(p, generator, population);
    }
    stats.close();
    histograms.close();
}

// Calculate the mean and variance of pref and trt and covariance of pref and trt.
void Simulation::statistics(const Parameters& p,
                            std::vector<Individual>& population,
                            std::ofstream& stats) {
    const int pop_size{static_cast<int>(p.get_pop_size())};
    const std::vector<double> prefs = collect_prefs(population);
    const double mean_pref = mean(prefs);
    const std::vector<double> trts = collect_trts(population);
    const double mean_trt = mean(trts);
    const double pref_variance = (sum(square_vector(prefs)) -
                                    (sum(prefs) * sum(prefs)) / pop_size)
                                 / pop_size;
    const double trt_variance = (sum(square_vector(trts)) -
                                    (sum(trts)  * sum(trts)) / pop_size)
                                 / pop_size;
    std::vector<double> pref_times_trt(population.size());
    std::transform(prefs.begin(), prefs.end(), trts.begin(), pref_times_trt.begin(),
                   std::multiplies<double>());
    const double covariance = (sum(pref_times_trt) -
                                    ((sum(prefs) * sum(trts)) / pop_size))
                                / pop_size;
    std::cout << "mean_pref " << mean_pref <<
                 " mean_trt " << mean_trt <<
                 " pref_variance " << pref_variance <<
                 " trt_variance " << trt_variance <<
                 " covariance " << covariance << std::endl;
    stats << mean_pref << ','
          << mean_trt << ','
          << pref_variance << ','
          << trt_variance << ','
          << covariance << std::endl;
}


/* Create two histograms, one of preferences in the population and one of male traits.
 * Possible values for the histogram run from all -1 to all +1 so the size is the difference
 * between the two, i.e. the all -1, all +1 plus the all 0 state.
 *
 * This currently doesn't work as I'm trying to get the histograms to be the right size.
 * As in half the size they were due to anychange in a gene shifts the number by 2.
 */
void Simulation::histogram(const Parameters& p,
                           std::vector<Individual>& population,
                           std::ofstream& histograms) {
    const int pop_size{static_cast<int>(p.get_pop_size())};
    const int n_pref_genes{static_cast<int>(p.get_n_pref_genes())};
    const int n_trt_genes{static_cast<int>(p.get_n_trt_genes())};
    const double scale_preference{static_cast<double>(p.get_scale_preference())};
    const double scale_trait{static_cast<double>(p.get_scale_trait())};
    const double pref_genes{static_cast<double>(n_pref_genes)};
    const double trt_genes{static_cast<double>(n_trt_genes)};
    std::vector<double> pref_hist(n_pref_genes + 1);
    std::vector<double> trt_hist(n_trt_genes + 1);
    for (int i = 0; i < pop_size; ++i) {
        for (double h = 0 - pref_genes; h < pref_genes + 3; h += 2) {
            if (population[i].get_preference() * scale_preference * pref_genes == h) {
                ++pref_hist[h + n_pref_genes];
                break;
            }
        }
        for (double h = 0 - trt_genes; h < trt_genes + 3; h += 2) {
            if (population[i].get_trait() * scale_trait * trt_genes == h) {
                ++trt_hist[h + n_trt_genes];
                break;
            }
        }
    }
    output_pref_histogram(p, pref_hist, histograms);
    output_trt_histogram(p, trt_hist, histograms);
}

void Simulation::output_pref_histogram(const Parameters& p,
                                      const std::vector<double>& pref_hist,
                                      std::ofstream& histograms) {
    std::cout << "Preference_Value ";
    histograms << "Preference_Value,";
    const int n_pref_genes{static_cast<int>(p.get_n_pref_genes())};
    const int scale_preference{static_cast<int>(p.get_scale_preference())};
    for (int h = 1 - n_pref_genes; h < n_pref_genes + 1; h += 2) {
        std::cout << h / scale_preference * n_pref_genes << ' ';
        histograms << h / scale_preference * n_pref_genes << ',';
    }
    std::cout << "\nPreference_Histogram ";
    histograms << "\nPreference_Histogram,";
    const int pref_hist_size{static_cast<int>(pref_hist.size())};
    for (int i = 0; i < pref_hist_size; ++i) {
        std::cout << pref_hist[i] << " ";
        histograms << pref_hist[i] << ",";
    }
}

void Simulation::output_trt_histogram(const Parameters& p,
                                      const std::vector<double>& trt_hist,
                                      std::ofstream& histograms) {
    std::cout << "\nTrait_Value ";
    histograms << "\nTrait_Value,";
    const int n_trt_genes{static_cast<int>(p.get_n_trt_genes())};
    const int scale_trait{static_cast<int>(p.get_scale_trait())};
    for (int h = 1 - n_trt_genes; h < n_trt_genes + 1; h += 2) {
        std::cout << h / scale_trait * n_trt_genes << ' ';
        histograms << h / scale_trait * n_trt_genes << ',';
    }
    std::cout << "\nTrait_Histogram ";
    histograms << "\nTrait_Histogram,";
    const int trt_hist_size{static_cast<int>(trt_hist.size())};
    for (int i = 0; i < trt_hist_size; ++i) {
        std::cout << trt_hist[i] << " ";
        histograms << trt_hist[i] << ",";
    }
    std::cout << std::endl;
    histograms << std::endl;
}

std::vector<Individual> Simulation::create_next_gen(const Parameters& p,
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
                            const Parameters& p,
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

// separate out a function to call for attractivity calculation. Have a separate one to turn off quality?
int Simulation::pick_father(std::mt19937& generator,
                            const Parameters& p,
                            std::vector<Individual>& population,
                            const int& mother) {
    const int pop_size{static_cast<int>(p.get_pop_size())};
    std::vector<double> male_viab_dist(pop_size);
    crt_male_viability(population, male_viab_dist, p.get_optimal_trait(), p.get_value_of_trait());
    std::vector<double> attractivity(pop_size);
    for (int i = 0; i < pop_size; ++i) {
        attractivity[i] = male_viab_dist[i] *
                exp(population[mother].get_preference() *
                    population[i].get_trait()); /* *
                    population[i].get_quality());*/
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

std::vector<double> collect_prefs(const std::vector<Individual>& population) {
    int pop_size{static_cast<int>(population.size())};
    std::vector<double> v(pop_size);
    for (int i = 0; i < pop_size; ++i) {
        v[i] = population[i].get_preference();
    }
    return v;
}

std::vector<double> collect_trts(const std::vector<Individual>& population) {
    int pop_size{static_cast<int>(population.size())};
    std::vector<double> v(pop_size);
    for (int i = 0; i < pop_size; ++i) {
        v[i] = population[i].get_trait();
    }
    return v;
}

double sum(const std::vector<double> v) {
    return std::accumulate(
                std::begin(v),
                std::end(v),
                0.0
                );
}

std::vector<double> square_vector(const std::vector<double>& v) {
    int pop_size{static_cast<int>(v.size())};
    std::vector<double> n(pop_size);
    std::transform(v.begin(), v.end(), v.begin(), n.begin(), std::multiplies<double>());
    return n;
}
