/*	jaan_main.cpp
    Written by James Andrews on 17-10-2016
    A simulation of a population with sexual selection.
    Based on Cichlids in Lake Victoria.

    ASSUMPTIONS
    Haploid, hermaphroditic population
    Multiple loci for trait and preference.
    Sequential, fixed interval sampling in mate selection.
    Fixed, discrete generations.
*/

#include <random>
#include "jaan_individual.h"
#include "jaan_parameters.h"
#include "jaan_simulation.h"

int main() {
    Parameters p(10000,   // Max generations
                 1000,    // population size
                 20,      // number of preference genes
                 20,      // number of trait genes
                 10,      // Number of quality genes
                 0.0,     // optimum preference
                 0.0,     // optimum trait
                 100,     // value of preference
                 1,       // value of trait
                 1.0e-3,  // Pref and trt mutation rate
                 1.0e-4,  // Chance quality gene goes up
                 1.0e-2,  // Chance quality gene goes down
                 1.2,     // Maximum preference value
                 3        // Maximum trait value
                 );
    std::mt19937 generator;
    Simulation simulation;
    simulation.run(p, generator);
    return 0;
}
