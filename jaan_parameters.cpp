#include <stdexcept>
#include "jaan_parameters.h"

Parameters::Parameters(                          //!OCLINT
        const int init_max_generations,
        const int init_pop_size,
        const int init_n_pref_genes,
        const int init_n_trt_genes,
        const int init_n_qual_genes,
        const double init_pref_and_trt_mu,
        const double init_quality_inc_mu,
        const double init_quality_dec_mu,
        const double init_scale_pref,
        const double init_scale_trait,
        const double init_migration_rate) :
    max_generations(init_max_generations),
    pop_size(init_pop_size),
    n_pref_genes(init_n_pref_genes),
    n_trt_genes(init_n_trt_genes),
    n_qual_genes(init_n_qual_genes),
    pref_and_trt_mu(init_pref_and_trt_mu),
    quality_inc_mu(init_quality_inc_mu),
    quality_dec_mu(init_quality_dec_mu),
    scale_preference(init_scale_pref),
    scale_trait(init_scale_trait),
    migration_rate(init_migration_rate)
{
    /// Check rates are between 0 and 1 and calls init_test_counts
    if (init_test_counts() || init_test_props())
        throw std::invalid_argument( "Input values for Parameters invalid, check assumptions." );
}

int Parameters::get_max_generations() const
{
    return max_generations;
}

int Parameters::get_pop_size() const
{
    return pop_size;
}

int Parameters::get_n_pref_genes() const
{
    return n_pref_genes;
}

int Parameters::get_n_trt_genes() const
{
    return n_trt_genes;
}

int Parameters::get_n_qual_genes() const
{
    return n_qual_genes;
}

double Parameters::get_pref_and_trt_mu() const
{
   return pref_and_trt_mu;
}

double Parameters::get_quality_inc_mu() const
{
    return quality_inc_mu;
}

double Parameters::get_quality_dec_mu() const
{
    return quality_dec_mu;
}

double Parameters::get_scale_preference() const
{
    return scale_preference;
}

double Parameters::get_scale_trait() const
{
    return scale_trait;
}

double Parameters::get_migration_rate() const
{
    return migration_rate;
}

/// Prints the parameters of the simulation to the output file.
void Parameters::print_parameters(std::ofstream& output) const
{
    output << "parameters\n"
           << "max_generations," << max_generations
           << "\npop_size," << pop_size
           << "\nn_pref_genes," << n_pref_genes
           << "\nn_trt_genes," << n_trt_genes
           << "\nn_qual_genes," << n_qual_genes
           << "\npref_and_trt_mu," << pref_and_trt_mu
           << "\nquality_inc_mu," << quality_inc_mu
           << "\nquality_dec_mu," << quality_dec_mu
           << "\nscale_preference," << scale_preference
           << "\nscale_trait," << scale_trait
           << "\nmigration_rate," << migration_rate << '\n';
}

/// Checks counts are not negative.
bool Parameters::init_test_counts() const
{
    return max_generations < 0 ||
           pop_size < 0 ||
           n_pref_genes < 0 ||
           n_trt_genes < 0 ||
           n_qual_genes < 0;
}

/// Checks proportions are between 0 and 1.
bool Parameters::init_test_props() const
{
    return pref_and_trt_mu < 0 ||
           pref_and_trt_mu > 1 ||
           quality_inc_mu < 0 ||
           quality_inc_mu > 1 ||
           quality_dec_mu < 0 ||
           quality_dec_mu > 1 ||
           migration_rate < 0 ||
           migration_rate > 1;
}
