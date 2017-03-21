#include <stdexcept>
#include "jaan_parameters.h"

Parameters::Parameters(                          //!OCLINT
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
        double init_quality_viab) :
    max_generations(init_max_generations),
    pop_size(init_pop_size),
    n_pref_genes(init_n_pref_genes),
    n_trt_genes(init_n_trt_genes),
    n_qual_genes(init_n_qual_genes),
    optimal_preference(init_optimal_pref),
    optimal_trait(init_optimal_trait),
    selection_on_pref(init_selection_on_pref),
    selection_on_trt(init_selection_on_trt),
    /// Calculates the number of mutations that ought to occur for preferences.
    pref_mu(init_pref_and_trt_mu * init_n_pref_genes * init_pop_size),// / 2),
    /// Calculates the number of mutations that ought to occur for traits.
    trt_mu(init_pref_and_trt_mu * init_n_trt_genes * init_pop_size),// / 2),
    /// Calculates the number of beneficial mutations that ought to occur for quality.
    quality_inc_mu(init_n_qual_genes * init_pop_size * init_quality_inc_mu),
    /// Calculates the number of detrimental mutations that ought to occur for quality.
    quality_dec_mu(init_n_qual_genes * init_pop_size * init_quality_dec_mu),
    scale_preference(init_scale_pref),
    scale_trait(init_scale_trait),
    quality_attr(init_quality_attr),
    quality_viab(init_quality_viab)
{
    /// Check rates are between 0 and 1 and calls init_test_counts
    if (init_test_counts() ||
            init_pref_and_trt_mu < 0 ||
            init_pref_and_trt_mu > 1 ||
            init_quality_inc_mu < 0 ||
            init_quality_inc_mu > 1 ||
            init_quality_dec_mu < 0 ||
            init_quality_dec_mu > 1)
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

double Parameters::get_optimal_preference() const
{
    return optimal_preference;
}

double Parameters::get_optimal_trait() const
{
    return optimal_trait;
}

double Parameters::get_selection_on_pref() const
{
    return selection_on_pref;
}

double Parameters::get_selection_on_trt() const
{
    return selection_on_trt;
}

double Parameters::get_pref_mu() const
{
   return pref_mu;
}

double Parameters::get_trt_mu() const
{
    return trt_mu;
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

double Parameters::get_quality_attr() const
{
    return quality_attr;
}

double Parameters::get_quality_viab() const
{
    return quality_viab;
}

/// Prints the parameters of the simulation to the output file.
void Parameters::print_parameters(std::ofstream& output) const
{
    output << "max_generations," << max_generations << '\n'
           << "pop_size," << pop_size << '\n'
           << "n_pref_genes," << n_pref_genes << '\n'
           << "n_trt_genes," << n_trt_genes << '\n'
           << "n_qual_genes," << n_qual_genes << '\n'
           << "optimal_preference," << optimal_preference << '\n'
           << "optimal_trait," << optimal_trait << '\n'
           << "selection_on_pref," << selection_on_pref << '\n'
           << "selection_on_trt," << selection_on_trt << '\n'
           << "pref_mu," << pref_mu / (pop_size * n_pref_genes) << '\n'
           << "trt_mu," << trt_mu / (pop_size * n_trt_genes) << '\n'
           << "quality_inc_mu," << quality_inc_mu / (pop_size * n_qual_genes) << '\n'
           << "quality_dec_mu," << quality_dec_mu / (pop_size * n_qual_genes) << '\n'
           << "scale_preference," << scale_preference << '\n'
           << "scale_trait," << scale_trait << '\n'
           << "quality_attr," << quality_attr << '\n'
           << "quality_viab," << quality_viab << '\n';
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

