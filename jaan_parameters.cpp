#include <stdexcept>
#include "jaan_parameters.h"

Parameters::Parameters(int max_generations,        // How many generations? //!OCLINT
                       int pop_size,               // How many individuals in each generation?
                       int n_pref_genes,           // Number of genes for Preference.
                       int n_trt_genes,            // Number of genes for Trait.
                       int n_qual_genes,           // Number of genes for Quality.
                       double optimal_preference,  // Optimal preference value for viability.
                       double optimal_trait,       // Optimal trait value for viability.
                       double value_of_preference,  // Higher = Lower cost of choosiness.
                       // Higher = Lower cost of trait. value_of_trait << value_of_preference
                       double value_of_trait,
                       double pref_and_trt_mu,
                       double quality_inc_mu,
                       double quality_dec_mu) :    // Mutation rate for preference and trait genes.
    max_generations(max_generations),
    pop_size(pop_size),
    n_pref_genes(n_pref_genes),
    n_trt_genes(n_trt_genes),
    n_qual_genes(n_qual_genes),
    optimal_preference(optimal_preference),
    optimal_trait(optimal_trait),
    value_of_preference(value_of_preference),
    value_of_trait(value_of_trait),
    pref_and_trt_mu(pref_and_trt_mu),
    quality_inc_mu(quality_inc_mu),
    quality_dec_mu(quality_dec_mu)
{
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

