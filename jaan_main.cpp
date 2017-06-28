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
#include <stdexcept>
#include "jaan_individual.h"
#include "jaan_parameters.h"
#include "jaan_habitat.h"
#include "jaan_simulation.h"

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::invalid_argument("Arg1: File for output of statistics.\n"
                              "Arg2: File for output of histograms.\n"
                              "Arg3: Parameters for the program.\n"
                              "Arg4: Habitat specific parameters for the program.");
    }
    std::ofstream happy("happy.txt");
    std::vector<int> input1(5);
    std::vector<double> input2(6);
    std::ifstream param_input;
    param_input.open(argv[3]);
    param_input >> input1[0] >> input1[1] >> input1[2] >> input1[3] >> input1[4]
                >> input2[0] >> input2[1] >> input2[2] >> input2[3] >> input2[4] >> input2[5];
    happy << input1[0] << input1[1] << input1[2] << input1[3] << input1[4] << input2[0]
              << input2[1] << input2[2] << input2[3] << input2[4] << input2[5] << std::endl;
    Parameters p(10,//000,     /// Max generations
                 20,//00,      /// population size (1000)
                 40,        /// number of preference genes (20)
                 40,        /// Number of quality genes
                 40,        /// number of trait genes (20)
                 1.0e-4,    /// Pref and trait mutation rate
                 1.0e-4,    /// Quality Mutation rate
                 40.0,      /// Maximum preference value
                 40.0,      /// Maximum quality value
                 40.0,      /// Maximum trait value
                 0.0        /// Rate of migration between the two habitats
                 );
    std::vector<Habitat> habitat_list;
    habitat_list.reserve(2);
    std::vector<double> input3(14);
    std::ifstream hab_input;
    hab_input.open(argv[4]);
    hab_input >> input3[0] >> input3[1] >> input3[2] >> input3[3] >> input3[4]
              >> input3[5] >> input3[6] >> input3[7] >> input3[8] >> input3[9]
              >> input3[10] >> input3[11] >> input3[12] >> input3[13];
    happy << input3[0] << input3[1] << input3[2] << input3[3] << input3[4]
              << input3[5] << input3[6] << input3[7] << input3[8] << input3[9]
              << input3[10] << input3[11] << input3[12] << input3[13] << std::endl;
    happy.close();
    {
        Habitat habitat0(0.0,   /// optimum preference
                         8.0,   /// optimum quality
                         0.0,   /// optimum trait
                         40.0,  /// stabilising selection on choosiness
                         2.0,   /// stabilising selection on viability
                         16.0,  /// stabilising selection on ornament
                         2.0    /// Efficiency of conversion from quality to signal
                         );
        Habitat habitat1(0.0,   /// optimum preference
                         -8.0,  /// optimum quality
                         0.0,   /// optimum trait
                         40.0,  /// stabilising selection on choosiness
                         2.0,   /// stabilising selection on viability
                         16.0,  /// stabilising selection on ornament
                         2.0    /// Efficiency of conversion from quality to signal
                         );
        habitat_list.push_back(habitat0);
        habitat_list.push_back(habitat1);
    }
    std::mt19937 generator;
    Simulation simulation;
    simulation.run(generator, p, habitat_list, argv[1], argv[2]);
    return 0;
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
