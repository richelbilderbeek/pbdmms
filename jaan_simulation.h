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
    void data_collection(Parameters& p,
                         std::vector<Individual>& population,
                         std::ofstream& output);
    double female_viability(Parameters& p,
                            std::mt19937& generator,
                            std::vector<Individual>& population);
    int mother_choosing(Parameters& p,
                        std::vector<Individual>& population,
                        double chosen);
    std::vector<Individual> create_next_gen(Parameters& p,
                                            std::mt19937& generator,
                                            std::vector<Individual>& population,
                                            std::uniform_real_distribution<> pickMother);
    void run(Parameters& p, std::mt19937& generator);
};

#endif // JAAN_SIMULATION_H
