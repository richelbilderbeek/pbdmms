#include <random>
#include <vector>
#include <boost/test/unit_test.hpp>
#include "jaan_individual.h"
#include "jaan_parameters.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

BOOST_AUTO_TEST_CASE(jaan_individual_generation_use) {
    Parameters p(10000, 1000, 20, 20, 10, 0.0, 0.0, 1, 0.01, 1.0e-3, 1.0e-4, 1.0e-2, 1.2, 3, 2, 1);
    const Individual i(p);
    const Individual j(i);
    BOOST_CHECK(i == j);
    std::mt19937 generator1(42);
    const Individual k(generator1, p, i, j);
    std::mt19937 generator2(42);
    const Individual l(generator2, p, i, j);
    BOOST_CHECK(k == l);
}

BOOST_AUTO_TEST_CASE(jaan_individual_mateSelect_use) {
}

#pragma GCC diagnostic pop



