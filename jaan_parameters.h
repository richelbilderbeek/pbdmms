#ifndef JAAN_PARAMETERS_H
#define JAAN_PARAMETERS_H
#include <fstream>

class Parameters
{
public:
    Parameters(
        const int init_max_generations,
        const int init_pop_size,
        const int init_n_pref_genes,
        const int init_n_qual_genes,
        const int init_n_trt_genes,
        const double init_pref_and_trt_mu,
        const double init_quality_mu,
        const double init_scale_pref,
        const double init_scale_quality,
        const double init_scale_trait,
        const double init_migration_rate);
    int get_max_generations() const;
    int get_pop_size() const;
    int get_n_pref_genes() const;
    int get_n_trt_genes() const;
    int get_n_qual_genes() const;
    double get_pref_and_trt_mu() const;
    double get_quality_mu() const;
    double get_scale_preference() const;
    double get_scale_trait() const;
    double get_scale_quality() const;
    double get_migration_rate() const;
    void print_parameters(std::ofstream& output) const;
private:
    bool init_test_counts() const;      /// Test that counts are positive.
    bool init_test_props() const;       /// Test that proportions are between 0 and 1.
    const int max_generations;          /// How many generations per simulation?
    const int pop_size;                 /// How many individuals in each generation?
    const int n_pref_genes;             /// Number of genes for Preference.
    const int n_qual_genes;             /// Number of genes for Quality.
    const int n_trt_genes;              /// Number of genes for Trait.
    const double pref_and_trt_mu;       /// Mutation rate for preference and trait genes.
    const double quality_mu;            /// Mutation rate for quality gene.
    const double scale_preference;      /// Maximum preference value.
    const double scale_quality;         /// Maximum quality value.
    const double scale_trait;           /// Maximum trait value.
    const double migration_rate;        /// Rate of migration between the two habitats.
};

#endif // JAAN_PARAMETERS_H
