#include <stdexcept>
#include "jaan_parameters.h"

Parameters::Parameters(int max_generations, // How many generations?
                       int pop_size, // How many individuals in each generation?
                       int n_pref_genes, // Number of genes for Preference.
                       int n_trt_genes, // Number of genes for Trait.
                       double optimal_preference, // Optimal preference value for viability.
                       double optimal_trait, // Optimal trait value for viability.
                       double cost_of_preference, // Higher = Lower cost of choosiness.
                       // Higher = Lower cost of trait. cost_of_trait << cost_of_preference
                       double cost_of_trait,
                       double mu) : // Mutation rate.
    max_generations(max_generations),
    pop_size(pop_size),
    n_pref_genes(n_pref_genes),
    n_trt_genes(n_trt_genes),
    optimal_preference(optimal_preference),
    optimal_trait(optimal_trait),
    cost_of_preference(cost_of_preference),
    cost_of_trait(cost_of_trait),
    mu(mu)
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

double Parameters::get_optimal_preference() {
    return optimal_preference;
}

double Parameters::get_optimal_trait() {
    return optimal_trait;
}

double Parameters::get_cost_of_preference() {
    return cost_of_preference;
}

double Parameters::get_cost_of_trait() {
    return cost_of_trait;
}

double Parameters::get_mu() {
    return mu;
}

