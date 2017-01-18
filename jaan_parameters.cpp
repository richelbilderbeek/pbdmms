#include <stdexcept>
#include "jaan_parameters.h"

Parameters::Parameters() :
    max_generations(1000),      // How many generations?
    pop_size(1000),             // How many individuals in each generation?
    n_pref_genes(10),           // Number of genes for Preference.
    n_trt_genes(10),            // Number of genes for Trait.
    trait_cost(0.0),            // How dangerous it is to have a Trait.
    optimal_preference(0.0),    // Optimal preference value for viability.
    optimal_trait(0.0),         // Optimal trait value for viability.
    cost_of_preference(1),      // Higher = Lower cost of choosiness.
    cost_of_trait(0.09),        // Higher = Lower cost of trait. cost_of_trait << cost_of_preference
    mu(1.0e-4)                  // Mutation rate.
{
}

void Parameters::set_max_generations(int input) {
    if (input < 0)
        throw std::invalid_argument("Final generation must be a positive integer.");
    max_generations = input;
}

void Parameters::set_pop_size(int input) {
    if (input < 0)
        throw std::invalid_argument("Population size must be a positive integer.");
    pop_size = input;
}

void Parameters::set_n_pref_genes(int input) {
    if (input < 0)
        throw std::invalid_argument("Number of preference genes must be a positive integer.");
    n_pref_genes = input;
}

void Parameters::set_n_trt_genes(int input) {
    if (input < 0)
        throw std::invalid_argument("Number of trait genes must be a positive integer.");
    n_trt_genes = input;
}

void Parameters::set_trait_cost(double input) {
    trait_cost = input;
}

void Parameters::set_optimal_preference(double input) {
    optimal_preference = input;
}

void Parameters::set_optimal_trait(double input) {
    optimal_trait = input;
}

void Parameters::set_cost_of_preference(double input) {
    cost_of_preference = input;
}

void Parameters::set_cost_of_trait(double input) {
    cost_of_trait = input;
}

void Parameters::set_mu(double input) {
    mu = input;
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

double Parameters::get_trait_cost() {
    return trait_cost;
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

