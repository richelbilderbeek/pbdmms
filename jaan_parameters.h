#ifndef JAAN_PARAMETERS_H
#define JAAN_PARAMETERS_H
#include <fstream>

class Parameters
{
public:
    Parameters(
            int init_max_generations,
            int init_pop_size,
            int init_n_pref_genes,
            int init_n_trt_genes,
            int init_n_qual_genes,
            double init_optimal_pref,
            double init_optimal_trait,
            double init_selection_on_pref,
            double init_selection_on_trt,
            double init_pref_and_trt_mu,
            double init_quality_inc_mu,
            double init_quality_dec_mu,
            double init_scale_pref,
            double init_scale_trait,
            double init_quality_attr,
            double init_quality_viab);
    int get_max_generations() const;
    int get_pop_size() const;
    int get_n_pref_genes() const;
    int get_n_trt_genes() const;
    int get_n_qual_genes() const;
    double get_optimal_preference() const;
    double get_optimal_trait() const;
    double get_selection_on_pref() const;
    double get_selection_on_trt() const;
    double get_pref_mu() const;
    double get_trt_mu() const;
    double get_quality_inc_mu() const;
    double get_quality_dec_mu() const;
    double get_scale_preference() const;
    double get_scale_trait() const;
    double get_quality_attr() const;
    double get_quality_viab() const;
    void print_parameters(std::ofstream& output) const;
private:
    bool init_test_counts() const;      // Test that counts are positive.
    const int max_generations;          // How many generations per simulation?
    const int pop_size;                 // How many individuals in each generation?
    const int n_pref_genes;             // Number of genes for Preference.
    const int n_trt_genes;              // Number of genes for Trait.
    const int n_qual_genes;             // Number of genes for Quality.
    const double optimal_preference;    // Optimal preference value for viability.
    const double optimal_trait;         // Optimal trait value for viability.
    const double selection_on_pref;     // Higher = Lower cost of choosiness.
    const double selection_on_trt;      // Higher = Lower cost of trait.
    // selection_on_trt << selection_on_pref.
    const double pref_mu;               // Mutation rate for preference genes.
    const double trt_mu;                // Mutation rate for trait genes.
    const double quality_inc_mu;        // Mutation rate for +1 to quality gene.
    const double quality_dec_mu;        // Mutation rate for -1 to quality gene.
    const double scale_preference;      // Maximum preference value.
    const double scale_trait;           // Maximum trait value.
    const double quality_attr;          // Effect of quality on attractiveness.
    const double quality_viab;          // Effect of quality on viability.
};

#endif // JAAN_PARAMETERS_H
