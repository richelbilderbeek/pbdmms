#include "jaan_parameters.h"
#include <boost/test/unit_test.hpp>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

BOOST_AUTO_TEST_CASE(jaan_parameters_use) {
    // Correct use of Parameters ought to be that it only calls stuff from Parameters, never sets things.
}

BOOST_AUTO_TEST_CASE(jaan_parameters_abuse) {
    // Incorrect use of Parameters would be setting the initial values of counts of things to negative numbers.
    // Value of preference ought to be much larger than value of trait, display warning?.
    // mutation rates MUST be between 0 and 1.
  Parameters p(-10000,   // Max generations
               -1000,    // population size
               -20,      // number of preference genes
               -20,      // number of trait genes
               -10,      // Number of quality genes
               0.0,     // optimum preference
               0.0,     // optimum trait
               1,       // value of preference
               0.01,    // value of trait
               2,  // Pref and trt mutation rate
               -4,  // Chance quality gene goes up
               8); // Chance quality gene goes down
}

#pragma GCC diagnostic pop



