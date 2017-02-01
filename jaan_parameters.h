#ifndef JAAN_PARAMETERS_H
#define JAAN_PARAMETERS_H

class Parameters
{
public:
    Parameters(int init_max_generations,
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
               double init_quality_dec_mu,
               double init_scale_pref,
               double init_scale_trait);
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
    double get_scale_preference();
    double get_scale_trait();
private:
    bool init_test_counts();           // Test that counts are positive.
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
    const double scale_preference;    // Maximum preference value.
    const double scale_trait;         // Maximum trait value.
};

#endif // JAAN_PARAMETERS_H
