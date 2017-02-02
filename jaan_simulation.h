#ifndef JAAN_SIMULATION_H
#define JAAN_SIMULATION_H

#include <vector>
#include <random>
#include "jaan_parameters.h"
#include "jaan_individual.h"

class Simulation
{
public:
    Simulation();
    void run(const Parameters& p,
             std::mt19937& generator);
    void statistics(const Parameters& p,
                    std::vector<Individual>& population,
                    std::ofstream& stats);
    void histogram(const Parameters& p,
                   std::vector<Individual>& population,
                   std::ofstream& histograms);
    void output_pref_histogram(const Parameters& p,
                               const std::vector<double>& pref_hist,
                               std::ofstream& histograms);
    void output_trt_histogram(const Parameters& p,
                              const std::vector<double>& trt_hist,
                              std::ofstream& histograms);
    std::vector<Individual> create_next_gen(const Parameters& p,
                                            std::mt19937& generator,
                                            std::vector<Individual>& population);
    int pick_mother(std::mt19937& generator,
                    const Parameters& p,
                    std::vector<Individual>& population);
    int pick_father(std::mt19937& generator,
                    const Parameters& p,
                    std::vector<Individual>& population,
                    const int& mother);
    void crt_female_viability(std::vector<Individual>& population,
                              std::vector<double>& viab_dist,
                              const double& optimal_preference,
                              const double& value_of_preference);
    void crt_male_viability(std::vector<Individual>& population,
                            std::vector<double>& viab_dist,
                            const double& optimal_trait,
                            const double& value_of_trait);
};

std::vector<double> collect_prefs(const std::vector<Individual>& population);
std::vector<double> collect_trts(const std::vector<Individual>& population);
std::vector<double> collect_quals(const std::vector<Individual>& population);
double compute_square(const double& x);
double sum(const std::vector<double> v);
std::vector<double> square_vector(const std::vector<double>& v);

#endif // JAAN_SIMULATION_H
