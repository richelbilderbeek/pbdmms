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
    void run(Parameters& p,
             std::mt19937& generator);
    void statistics(Parameters& p,
                    std::vector<Individual>& population,
                    std::ofstream& stats);
    void histogram(Parameters& p,
                   std::vector<Individual>& population,
                   std::ofstream& histograms);
    void output_histogram(const std::vector<double>& pref_hist,
                          const std::vector<double>& trt_hist,
                          std::ofstream& histograms);
    std::vector<Individual> create_next_gen(Parameters& p,
                                            std::mt19937& generator,
                                            std::vector<Individual>& population);
    int pick_mother(std::mt19937& generator,
                    Parameters& p,
                    std::vector<Individual>& population);
    int pick_father(std::mt19937& generator,
                    Parameters& p,
                    std::vector<Individual>& population,
                    const int& mother);
    void crt_viability(std::vector<Individual>& population,
                       std::vector<double>& female_viab_dist,
                       const double& optimal_trait,
                       const double& value_of_trait);
};

#endif // JAAN_SIMULATION_H
