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
        double init_value_of_pref,
        double init_value_of_trait,
        double init_pref_and_trt_mu,
        double init_quality_inc_mu,
        double init_quality_dec_mu,
        double init_scale_pref,
        double init_scale_trait,
        double init_quality_effect,
        double init_quality_viab) :
    max_generations(init_max_generations),
    pop_size(init_pop_size),
    n_pref_genes(init_n_pref_genes),
    n_trt_genes(init_n_trt_genes),
    n_qual_genes(init_n_qual_genes),
    optimal_preference(init_optimal_pref),
    optimal_trait(init_optimal_trait),
    value_of_preference(init_value_of_pref),
    value_of_trait(init_value_of_trait),
    pref_and_trt_mu(init_pref_and_trt_mu),
    quality_inc_mu(init_quality_inc_mu),
    quality_dec_mu(init_quality_dec_mu),
    scale_preference(init_scale_pref),
    scale_trait(init_scale_trait),
    quality_effect(init_quality_effect),
    quality_viab(init_quality_viab)
{
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

double Parameters::get_value_of_preference() const
{
    return value_of_preference;
}

double Parameters::get_value_of_trait() const
{
    return value_of_trait;
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

double Parameters::get_quality_effect() const
{
    return quality_effect;
}

double Parameters::get_quality_viab() const
{
    return quality_viab;
}

void Parameters::print_parameters(std::ofstream& stats) const
{
    stats << "max_generations" << max_generations << '\n'
          << "pop_size" << pop_size << '\n'
          << "n_pref_genes" << n_pref_genes << '\n'
          << "n_trt_genes" << n_trt_genes << '\n'
          << "n_qual_genes" << n_qual_genes << '\n'
          << "optimal_preference" << optimal_preference << '\n'
          << "optimal_trait" << optimal_trait << '\n'
          << "value_of_preference" << value_of_preference << '\n'
          << "value_of_trait" << value_of_trait << '\n'
          << "pref_and_trt_mu" << pref_and_trt_mu << '\n'
          << "quality_inc_mu" << quality_inc_mu << '\n'
          << "quality_dec_mu" << quality_dec_mu << '\n'
          << "scale_preference" << scale_preference << '\n'
          << "scale_trait" << scale_trait << '\n'
          << "quality_effect" << quality_effect << '\n'
          << "quality_viab" << quality_viab << '\n';
}

bool Parameters::init_test_counts() const
{
    return max_generations < 0 ||
           pop_size < 0 ||
           n_pref_genes < 0 ||
           n_trt_genes < 0 ||
           n_qual_genes < 0;
}

