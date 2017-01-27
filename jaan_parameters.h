#ifndef JAAN_PARAMETERS_H
#define JAAN_PARAMETERS_H

class Parameters
{
public:
    Parameters(int init_max_generations,        // How many generations per simulation?
               int init_pop_size,               // How many individuals in each generation?
               int init_n_pref_genes,           // Number of genes for Preference.
               int init_n_trt_genes,            // Number of genes for Trait.
               int init_n_qual_genes,           // Number of genes for quality.
               double init_optimal_pref,        // Optimal preference value for viability.
               double init_optimal_trait,       // Optimal trait value for viability.
               double init_value_of_pref,       // Higher = Lower cost of choosiness.
               // Higher = Lower cost of trait. value_of_trait << value_of_preference
               double init_value_of_trait,
               double init_pref_and_trt_mu,     // Mutation rate for preference and trait genes.
               double init_quality_inc_mu,      // Mutation rate for +1 to quality gene.
               double init_quality_dec_mu       // Mutation rate for -1 to quality gene.
    );
    int get_max_generations();
    int get_pop_size();
    int get_n_pref_genes();
    int get_n_trt_genes();
    int get_n_qual_genes();
    double get_optimal_preference();
    double get_optimal_trait();
    double get_value_of_preference();
    double get_value_of_trait();
    double get_pref_and_trt_mu();
    double get_quality_inc_mu();
    double get_quality_dec_mu();
private:
    const int max_generations;        // How many generations per simulation?
    const int pop_size;               // How many individuals in each generation?
    const int n_pref_genes;           // Number of genes for Preference.
    const int n_trt_genes;            // Number of genes for Trait.
    const int n_qual_genes;           // Number of genes for Quality.
    const double optimal_preference;  // Optimal preference value for viability.
    const double optimal_trait;       // Optimal trait value for viability.
    const double value_of_preference; // Higher = Lower cost of choosiness.
    // Higher = Lower cost of trait. value_of_trait << value_of_preference
    const double value_of_trait;
    const double pref_and_trt_mu;     // Mutation rate for preference and trait genes.
    const double quality_inc_mu;      // Mutation rate for +1 to quality gene.
    const double quality_dec_mu;      // Mutation rate for -1 to quality gene.
};

#endif // JAAN_PARAMETERS_H
