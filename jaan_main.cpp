/*	jaan_main.cpp
    Written by James Andrews on 17-10-2016
    A simulation of a population with sexual selection.
    Based on Cichlids in Lake Victoria.

    ASSUMPTIONS
    Haploid, hermaphroditic population
    Multiple loci for trait and preference.
    Sequential, fixed interval sampling in mate selection.
    Fixed, discrete generations.

    STEP SIZE AND SCALE NEED TO WORK HAND IN HAND WHEN YOU SET THEM.
*/
#include <iostream>
#include <random>
#include "jaan_individual.h"
#include "jaan_parameters.h"
#include "jaan_habitat.h"
#include "jaan_simulation.h"

int main()
{
    Parameters p(10000,     /// Max generations
                 2000,      /// population size (1000)
                 40,        /// number of preference genes (20)
                 40,        /// number of trait genes (20)
                 50,        /// Number of quality genes
                 1.0e-4,    /// Pref and trait mutation rate
                 1.0e-4,    /// Chance quality gene goes up
                 1.0e-2,    /// Chance quality gene goes down
                 40.0,      /// Maximum preference value
                 40.0,      /// Maximum trait value
                 0          /// Rate of migration between the two habitats
                 );
    std::vector<Habitat> habitat_list;
    habitat_list.reserve(2);
    {
        Habitat habitat0(4,     /// optimum preference
                         -6,    /// optimum trait
                         5.0,   /// stabilising selection on choosiness
                         2.0,   /// stabilising selection on ornament
                         2.0,   /// Efficiency of conversion from quality to signal
                         0.8,   /// Selection on viability
                         1      /// Clarity of the signal of trait
                         );
        Habitat habitat1(4,     /// optimum preference
                         -6,    /// optimum trait
                         5.0,   /// stabilising selection on choosiness
                         2.0,   /// stabilising selection on ornament
                         0.0,   /// Efficiency of conversion from quality to signal
                         0.8,   /// Selection on viability
                         1      /// Clarity of the signal of trait
                         );
        habitat_list.push_back(habitat0);
        habitat_list.push_back(habitat1);
    }
    std::mt19937 generator;
    Simulation simulation;
    simulation.run(generator, p, habitat_list, "efficis0inonehab.csv", "jaan_hist.csv");
/*    Parameters p1(10000,     /// Max generations
                  2000,      /// population size (1000)
                  40,        /// number of preference genes (20)
                  40,        /// number of trait genes (20)
                  50,        /// Number of quality genes
                  1.0e-4,    /// Pref and trait mutation rate
                  1.0e-4,    /// Chance quality gene goes up
                  1.0e-2,    /// Chance quality gene goes down
                  40.0,      /// Maximum preference value
                  40.0,      /// Maximum trait value
                  0.2        /// Rate of migration between the two habitats
                  );
    std::vector<Habitat> habitat_list1;
    habitat_list1.reserve(2);
    {
        Habitat habitat0(4,     /// optimum preference
                         -6,    /// optimum trait
                         5.0,   /// stabilising selection on choosiness
                         2.0,   /// stabilising selection on ornament
                         2.0,   /// Efficiency of conversion from quality to signal
                         0.8,   /// Selection on viability
                         1      /// Clarity of the signal of trait
                         );
        Habitat habitat1(-4,     /// optimum preference
                         6,    /// optimum trait
                         5.0,   /// stabilising selection on choosiness
                         2.0,   /// stabilising selection on ornament
                         2.0,   /// Efficiency of conversion from quality to signal
                         0.8,   /// Selection on viability
                         1      /// Clarity of the signal of trait
                         );
        habitat_list1.push_back(habitat0);
        habitat_list1.push_back(habitat1);
    }
    std::mt19937 generator1;
    simulation.run(generator1, p1, habitat_list1, "optimareversed.csv", "jaan_hist.csv");
    Parameters p2(10000,     /// Max generations
                  2000,      /// population size (1000)
                  40,        /// number of preference genes (20)
                  40,        /// number of trait genes (20)
                  50,        /// Number of quality genes
                  1.0e-4,    /// Pref and trait mutation rate
                  1.0e-4,    /// Chance quality gene goes up
                  1.0e-2,    /// Chance quality gene goes down
                  40.0,      /// Maximum preference value
                  40.0,      /// Maximum trait value
                  0.8        /// Rate of migration between the two habitats
                  );
    std::vector<Habitat> habitat_list2;
    habitat_list2.reserve(2);
    {
        Habitat habitat0(4,     /// optimum preference
                         -6,    /// optimum trait
                         5.0,   /// stabilising selection on choosiness
                         2.0,   /// stabilising selection on ornament
                         2.0,   /// Efficiency of conversion from quality to signal
                         0.8,   /// Selection on viability
                         1      /// Clarity of the signal of trait
                         );
        Habitat habitat1(4,     /// optimum preference
                         -6,    /// optimum trait
                         5.0,   /// stabilising selection on choosiness
                         2.0,   /// stabilising selection on ornament
                         2.0,   /// Efficiency of conversion from quality to signal
                         0.8,   /// Selection on viability
                         1      /// Clarity of the signal of trait
                         );
        habitat_list2.push_back(habitat0);
        habitat_list2.push_back(habitat1);
    }
    std::mt19937 generator2;
    simulation.run(generator2, p2, habitat_list2, "highmigration.csv", "jaan_hist.csv");
    Parameters p3(10000,     /// Max generations
                  2000,      /// population size (1000)
                  40,        /// number of preference genes (20)
                  40,        /// number of trait genes (20)
                  50,        /// Number of quality genes
                  1.0e-4,    /// Pref and trait mutation rate
                  1.0e-4,    /// Chance quality gene goes up
                  1.0e-2,    /// Chance quality gene goes down
                  40.0,      /// Maximum preference value
                  40.0,      /// Maximum trait value
                  0.2        /// Rate of migration between the two habitats
                  );
    std::vector<Habitat> habitat_list3;
    habitat_list3.reserve(2);
    {
        Habitat habitat0(4,     /// optimum preference
                         -6,    /// optimum trait
                         5.0,   /// stabilising selection on choosiness
                         2.0,   /// stabilising selection on ornament
                         2.0,   /// Efficiency of conversion from quality to signal
                         0.8,   /// Selection on viability
                         0.1    /// Clarity of the signal of trait
                         );
        Habitat habitat1(4,     /// optimum preference
                         -6,    /// optimum trait
                         5.0,   /// stabilising selection on choosiness
                         2.0,   /// stabilising selection on ornament
                         2.0,   /// Efficiency of conversion from quality to signal
                         0.8,   /// Selection on viability
                         1      /// Clarity of the signal of trait
                         );
        habitat_list3.push_back(habitat0);
        habitat_list3.push_back(habitat1);
    }
    std::mt19937 generator3;
    simulation.run(generator3, p3, habitat_list3, "loweredclarity.csv", "jaan_hist.csv");
*/    return 0;
}
/*{ Based on Chapter 8
    Parameters p(10000,   // Max generations
                 1000,    // population size (1000)
                 20,      // number of preference genes (20)
                 20,      // number of trait genes (20)
                 10,      // Number of quality genes
                 0,       // optimum preference
                 0,       // optimum trait
                 7.07,    // stabilising selection on choosiness
                 4,       // stabilising selection on ornament
                 1.0e-3,  // Pref and trt mutation rate
                 1.0e-4,  // Chance quality gene goes up
                 1.0e-2,  // Chance quality gene goes down
                 1.2,     // Maximum preference value
                 3,       // Maximum trait value
                 2,       // Quality effect on attractiveness
                 1        // Quality effect on viability
                 );
    std::mt19937 generator;
    Simulation simulation;
    simulation.run(generator, p);
    return 0;
}{ Sander's recommendations.
    Parameters p(10000,     // Max generations
                 1000,      // population size (1000)
                 40,        // number of preference genes (20)
                 40,        // number of trait genes (20)
                 50,        // Number of quality genes
                 0.5,       // optimum preference
                 -0.8,      // optimum trait
                 5.0,       // stabilising selection on choosiness
                 2.0,       // stabilising selection on ornament
                 1.0e-4,    // Pref and trt mutation rate
                 1.0e-4,    // Chance quality gene goes up
                 1.0e-2,    // Chance quality gene goes down
                 5.0,       // Maximum preference value
                 5.0,       // Maximum trait value
                 2.0,       // Quality effect on attractiveness TRY 0.0 and 2.0
                 0.8        // Quality effect on viability
                 );
    std::mt19937 generator;
    Simulation simulation;
    simulation.run(generator, p);
    return 0;
}*/
