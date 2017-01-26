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
    void statistics(Parameters& p,
                    std::vector<Individual>& population,
                    std::ofstream& stats);
    void histogram(Parameters& p,
                   std::vector<Individual>& population,
                   std::ofstream& histograms);
    void output_histogram(const std::vector<double>& pref_hist,
                          const std::vector<double>& trt_hist,
                          std::ofstream& histograms);
    double female_viability(Parameters& p,
                            std::vector<Individual>& population);
    int mother_choosing(Parameters& p,
                        std::vector<Individual>& population,
                        const double chosen);
    std::vector<Individual> create_next_gen(Parameters& p,
                                            std::mt19937& generator,
                                            std::vector<Individual>& population,
                                            std::uniform_real_distribution<> mother_distribution);
    void run(Parameters& p, std::mt19937& generator);
};

#endif // JAAN_SIMULATION_H
