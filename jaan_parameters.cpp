#include <stdexcept>
#include "jaan_parameters.h"

Parameters::Parameters(int init_max_generations,        //!OCLINT
                       int init_pop_size,
                       int init_n_pref_genes,
                       int init_n_trt_genes,
                       int init_n_qual_genes,
                       double init_optimal_pref,
                       double init_optimal_trait,
                       double init_value_of_pref,
                       double init_value_of_trait,
                       double init_pref_and_trt_mu,
                       double init_quality_inc_mu,
                       double init_quality_dec_mu) :
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
    if (init_test_counts() ||
        init_pref_and_trt_mu < 0 ||
        init_pref_and_trt_mu > 1 ||
        init_quality_inc_mu < 0 ||
        init_quality_inc_mu > 1 ||
        init_quality_dec_mu < 0 ||
        init_quality_dec_mu > 1)
        throw std::invalid_argument( "Input values for Parameters invalid, check assumptions." );
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

bool Parameters::init_test_counts() {
    return max_generations < 0 ||
           pop_size < 0 ||
           n_pref_genes < 0 ||
           n_trt_genes < 0 ||
           n_qual_genes < 0;
}

