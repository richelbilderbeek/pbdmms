#ifndef JAAN_PARAMETERS_H
#define JAAN_PARAMETERS_H

class Parameters
{
public:
    Parameters();
    void set_max_generations(int input);
    void set_pop_size(int input);
    void set_n_pref_genes(int input);
    void set_n_trt_genes(int input);
    void set_trait_cost(double input);
    void set_optimal_preference(double input);
    void set_optimal_trait(double input);
    void set_cost_of_preference(double input);
    void set_cost_of_trait(double input);
    void set_mu(double input);
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
    int max_generations;        // How many generations per simulation?
    int pop_size;               // How many individuals in each generation?
    int n_pref_genes;           // Number of genes for Preference.
    int n_trt_genes;            // Number of genes for Trait.
    double trait_cost;          // How dangerous it is to have a Trait.
    double optimal_preference;  // Optimal preference value for viability.
    double optimal_trait;       // Optimal trait value for viability.
    double cost_of_preference;  // Higher = Lower cost of choosiness.
    double cost_of_trait;       // Higher = Lower cost of trait. cost_of_trait << cost_of_preference
    double mu;                  // Mutation rate.
};

#endif // JAAN_PARAMETERS_H
