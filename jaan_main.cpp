/*	jaan_main.cpp
    Written by James Andrews on 17-10-2016
    A simulation of a population with sexual selection.
    Based on Cichlids in Lake Victoria.

    ASSUMPTIONS
    Haploid, hermaphroditic population
    Multiple loci for trait and preference.
    Sequential, fixed interval sampling in mate selection.
    Mate selection prefers bigger traits.
    Fixed, discrete generations.

    RIGHT NOW I'M CHANGING MATESELECT TO SCORE THE POPULATION ACCORDING TO PREFERENCE AND THEN
    IT OUGHT TO PICK MUCH LIKE THE PICKMOTHER SECTION WHICH SHOULD BE A FATHER.
    From here:
     - Weighted lottery for who is a mother based on vFemale.
     - Weighted lottery for who is a father based on vMale.
     - Mate choice expression exp(p(focal mother) * t(all fathers) * alpha(all fathers), who
       is best choice is when this expression is maximised but then she chooses based on this
       distribution. This changes mate selection to match preference.
*/

#include <random>
#include "jaan_individual.h"
#include "jaan_parameters.h"
#include "jaan_simulation.h"

int main() {
    Parameters p(1000,    // Max generations
                 1000,    // population size
                 20,      // number of preference genes
                 20,      // number of trait genes
                 10,      // Number of quality genes
                 0.0,     // optimum preference
                 0.0,     // optimum trait
                 1,       // value of preference
                 0.01,    // value of trait
                 1.0e-3,  // Pref and trt mutation rate
                 1.0e-4,  // Chance quality gene goes up
                 1.0e-2); // Chance quality gene goes down
    std::mt19937 generator;
    Simulation simulation;
    simulation.run(p, generator);
    return 0;
}
