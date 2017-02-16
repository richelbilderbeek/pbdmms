#include "jaan_parameters.h"
#include <boost/test/unit_test.hpp>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

/* Correct use of Parameters ought to be that it only
 * calls stuff from Parameters, never sets things.
 * How do you test that?
 */
BOOST_AUTO_TEST_CASE(jaan_parameters_use) {
}

/* optimum preference: Must be between +/- the scaling value for pref when implemented.
 * optimum trait: Must be between +/- the scaling value for trait when implemented.
 * value of preference: Should be a warning for when preference
 * is not an order of magnitude bigger than value of trait
 */
BOOST_AUTO_TEST_CASE(jaan_parameters_abuse) {
    // Test max_generations is positive.
    BOOST_CHECK_THROW(Parameters p(-1000, 1000, 20, 20, 10, 0.0, 0.0, 1,
                                   0.01, 1e-02, 1e-02, 1e-02, 1.2, 3, 2),
                      std::invalid_argument);
    // Test pop_size is positive.
    BOOST_CHECK_THROW(Parameters p(1000, -1000, 20, 20, 10, 0.0, 0.0, 1,
                                   0.01, 1e-02, 1e-02, 1e-02, 1.2, 3, 2),
                      std::invalid_argument);
    // Test n_pref_genes is positive.
    BOOST_CHECK_THROW(Parameters p(1000, 1000, -20, 20, 10, 0.0, 0.0, 1,
                                   0.01, 1e-02, 1e-02, 1e-02, 1.2, 3, 2),
                      std::invalid_argument);
    // Test n_trt_genes is positive.
    BOOST_CHECK_THROW(Parameters p(1000, 1000, 20, -20, 10, 0.0, 0.0, 1,
                                   0.01, 1e-02, 1e-02, 1e-02, 1.2, 3, 2),
                      std::invalid_argument);
    // Test n_qual_genes is positive.
    BOOST_CHECK_THROW(Parameters p(1000, 1000, 20, 20, -10, 0.0, 0.0, 1,
                                   0.01, 1e-02, 1e-02, 1e-02, 1.2, 3, 2),
                      std::invalid_argument);
    // Test pref_and_trt_mu is below 1.
    BOOST_CHECK_THROW(Parameters p(1000, 1000, 20, 20, 10, 0.0, 0.0, 1,
                                   0.01, 2, 1e-02, 1e-02, 1.2, 3, 2),
                      std::invalid_argument);
    // Test qual_inc_mu is below 1.
    BOOST_CHECK_THROW(Parameters p(1000, 1000, 20, 20, 10, 0.0, 0.0, 1,
                                   0.01, 1e-02, 2, 1e-02, 1.2, 3, 2),
                      std::invalid_argument);
    // Test qual_dec_mu is below 1.
    BOOST_CHECK_THROW(Parameters p(1000, 1000, 20, 20, 10, 0.0, 0.0, 1,
                                   0.01, 1e-02, 1e-02, 2, 1.2, 3, 2),
                      std::invalid_argument);
    // Test pref_and_trt_mu is above 0.
    BOOST_CHECK_THROW(Parameters p(1000, 1000, 20, 20, 10, 0.0, 0.0, 1,
                                   0.01, -1e-02, 1e-02, 1e-02, 1.2, 3, 2),
                      std::invalid_argument);
    // Test qual_inc_mu is above 0.
    BOOST_CHECK_THROW(Parameters p(1000, 1000, 20, 20, 10, 0.0, 0.0, 1,
                                   0.01, 1e-02, -1e-02, 1e-02, 1.2, 3, 2),
                      std::invalid_argument);
    // Test qual_dec_mu is above 0.
    BOOST_CHECK_THROW(Parameters p(1000, 1000, 20, 20, 10, 0.0, 0.0, 1,
                                   0.01, 1e-02, 1e-02, -1e-02, 1.2, 3, 2),
                      std::invalid_argument);
}

#pragma GCC diagnostic pop
