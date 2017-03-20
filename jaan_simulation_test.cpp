#include <numeric>
#include <algorithm>
#include <random>
#include <fstream>
#include "jaan_simulation.h"
#include "jaan_parameters.h"
#include <boost/test/unit_test.hpp>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

// Need to test the stats work correctly.
BOOST_AUTO_TEST_CASE(jaan_simulation_hist_use) {
    Parameters p(1000, 1000, 20, 20, 10, 0.0, 0.0, 1, 0.01,
                 1e-02, 1e-02, 1e-02, 1.2, 3, 2, 1);
    std::vector<Individual> population(p.get_pop_size(), Individual(p));
    Simulation simulation;
    std::mt19937 generator;
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    const int pop_size{static_cast<int>(p.get_pop_size())};
    for (int i = 0; i < pop_size; ++i) {
        /* These pass population[i].get_trt_genes() but I need to pass the reference to them.
         * Furthermore, I want to reset the generator immediately before this loop, muate, reset
         * the generator, mutate the other one, do the histograms and compare them to see if the
         * results are the same.
         *
         * This doesn't solve the issue of why the histograms are not summing to 1000 individuals.
         */
        std::vector<double> trt_genes = population[i].get_trt_genes();
        std::vector<double> pref_genes = population[i].get_pref_genes();
    }
    std::ofstream histograms("test_histograms.csv");
//    simulation.histogram(p, population, histograms);
    histograms.close();
}

BOOST_AUTO_TEST_CASE(jaan_simulation_covariance_use) {
    std::vector<double> v1(1000);
    std::iota(std::begin(v1), std::end(v1), 0.0);
    std::vector<double> v2(1000);
    std::iota(std::begin(v2), std::end(v2), 0.0);
    double covar = covariance_calc(v1, v2);
    BOOST_CHECK(covar == 83333.25);
    std::reverse(std::begin(v1), std::end(v1));
    covar = covariance_calc(v1, v2);
    BOOST_CHECK(covar == -83333.25);
}

BOOST_AUTO_TEST_CASE(jaan_simulation_variance_use) {
    std::vector<double> v(31, 0);
    BOOST_CHECK(variance_calc(v) == 0);
    std::cout << std::endl;
    std::iota(std::begin(v), std::end(v), 1);
    BOOST_CHECK(variance_calc(v) == 80);
}

#pragma GCC diagnostic pop
