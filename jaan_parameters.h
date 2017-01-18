#ifndef JAAN_PARAMETERS_H
#define JAAN_PARAMETERS_H

class Parameters
{
public:
    Parameters(int max_generations,        // How many generations per simulation?
               int pop_size,               // How many individuals in each generation?
               int n_pref_genes,           // Number of genes for Preference.
               int n_trt_genes,            // Number of genes for Trait.
               double optimal_preference,  // Optimal preference value for viability.
               double optimal_trait,       // Optimal trait value for viability.
               double cost_of_preference,  // Higher = Lower cost of choosiness.
               // Higher = Lower cost of trait. cost_of_trait << cost_of_preference
               double cost_of_trait,
               double mu                   // Mutation rate.
    );
    int get_max_generations();
    int get_pop_size();
    int get_n_pref_genes();
    int get_n_trt_genes();
    double get_trait_cost();
    double get_optimal_preference();
    double get_optimal_trait();
    double get_cost_of_preference();
    double get_cost_of_trait();
    double get_mu();
private:
    const int max_generations;        // How many generations per simulation?
    const int pop_size;               // How many individuals in each generation?
    const int n_pref_genes;           // Number of genes for Preference.
    const int n_trt_genes;            // Number of genes for Trait.
    const double optimal_preference;  // Optimal preference value for viability.
    const double optimal_trait;       // Optimal trait value for viability.
    const double cost_of_preference;  // Higher = Lower cost of choosiness.
    // Higher = Lower cost of trait. cost_of_trait << cost_of_preference
    const double cost_of_trait;
    const double mu;                  // Mutation rate.
};

#endif // JAAN_PARAMETERS_H
