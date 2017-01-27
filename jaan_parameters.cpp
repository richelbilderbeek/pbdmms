#include <stdexcept>
#include "jaan_parameters.h"

Parameters::Parameters(// How many generations per simulation?
                       int init_max_generations,        //!OCLINT
                       int init_pop_size,               // How many individuals in each generation?
                       int init_n_pref_genes,           // Number of genes for Preference.
                       int init_n_trt_genes,            // Number of genes for Trait.
                       int init_n_qual_genes,           // Number of genes for Quality.
                       double init_optimal_pref,        // Optimal preference value for viability.
                       double init_optimal_trait,       // Optimal trait value for viability.
                       double init_value_of_pref,       // Higher = Lower cost of choosiness.
                       // Higher = Lower cost of trait. value_of_trait << value_of_preference
                       double init_value_of_trait,
                       double init_pref_and_trt_mu,     // Pref and trait mutation rate.
                       double init_quality_inc_mu,      // Rate quality genes flip to 1.
                       double init_quality_dec_mu) :    // rate that quality genes flip to 0.
    max_generations(init_max_generations),
    pop_size(init_pop_size),
    n_pref_genes(init_n_pref_genes),
    n_trt_genes(init_n_trt_genes),
    n_qual_genes(init_n_qual_genes),
    optimal_preference(init_optimal_pref),
    optimal_trait(init_optimal_trait),
    value_of_preference(init_value_of_pref),
    value_of_trait(init_value_of_trait),
    pref_and_trt_mu(init_pref_and_trt_mu),
    quality_inc_mu(init_quality_inc_mu),
    quality_dec_mu(init_quality_dec_mu)
{
    if (init_max_generations < 0)
        throw std::invalid_argument( " max_generations must be positive. ");
    if (init_pop_size < 0)
        throw std::invalid_argument( " pop_size must be positive. ");
    if (init_n_pref_genes < 0)
        throw std::invalid_argument( " n_pref_genes must be positive. ");
    if (init_n_trt_genes < 0)
        throw std::invalid_argument( " n_trt_genes must be positive. ");
    if (init_n_qual_genes < 0)
        throw std::invalid_argument( " n_qual_genes must be positive. ");
    if (init_pref_and_trt_mu < 0 || init_pref_and_trt_mu > 1)
        throw std::invalid_argument( " pref_and_trt_mu must be between 0 and 1. ");
    if (init_quality_inc_mu < 0 || quality_inc_mu > 1)
        throw std::invalid_argument( " quality_inc_mu must be between 0 and 1. ");
    if (init_quality_dec_mu < 0 || init_quality_dec_mu > 1)
        throw std::invalid_argument( " quality_dec_mu must be between 0 and 1. ");
}

int Parameters::get_max_generations() {
    return max_generations;
}

int Parameters::get_pop_size() {
    return pop_size;
}

int Parameters::get_n_pref_genes() {
    return n_pref_genes;
}

int Parameters::get_n_trt_genes() {
    return n_trt_genes;
}

int Parameters::get_n_qual_genes() {
    return n_qual_genes;
}

double Parameters::get_optimal_preference() {
    return optimal_preference;
}

double Parameters::get_optimal_trait() {
    return optimal_trait;
}

double Parameters::get_value_of_preference() {
    return value_of_preference;
}

double Parameters::get_value_of_trait() {
    return value_of_trait;
}

double Parameters::get_pref_and_trt_mu() {
    return pref_and_trt_mu;
}

double Parameters::get_quality_inc_mu() {
    return quality_inc_mu;
}

double Parameters::get_quality_dec_mu() {
    return quality_dec_mu;
}

