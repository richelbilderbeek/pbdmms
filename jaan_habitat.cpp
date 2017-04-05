#include "jaan_habitat.h"

Habitat::Habitat(
        const double init_optimal_pref,
        const double init_optimal_trait,
        const double init_selection_on_pref,
        const double init_selection_on_trt,
        const double init_expr_efficiency,
        const double init_selection_on_quality,
        const double init_signal_clarity):
    optimal_preference(init_optimal_pref),
    optimal_trait(init_optimal_trait),
    selection_on_pref(init_selection_on_pref),
    selection_on_trt(init_selection_on_trt),
    expr_efficiency(init_expr_efficiency),
    selection_on_quality(init_selection_on_quality),
    signal_clarity(init_signal_clarity)
{
}

double Habitat::get_optimal_preference() const
{
    return optimal_preference;
}

double Habitat::get_optimal_trait() const
{
    return optimal_trait;
}

double Habitat::get_selection_on_pref() const
{
    return selection_on_pref;
}

double Habitat::get_selection_on_trt() const
{
    return selection_on_trt;
}

double Habitat::get_expr_efficiency() const
{
    return expr_efficiency;
}

double Habitat::get_selection_on_quality() const
{
    return selection_on_quality;
}

double Habitat::get_signal_clarity() const
{
    return signal_clarity;
}

/// Prints the parameters of the simulation to the output file.
void Habitat::print_habitat(std::ofstream& output) const
{
    output << "optimal_preference," << optimal_preference
           << "\noptimal_trait," << optimal_trait
           << "\nselection_on_pref," << selection_on_pref
           << "\nselection_on_trt," << selection_on_trt
           << "\nexpr_efficiency," << expr_efficiency
           << "\nselection_on_quality," << selection_on_quality
           << "\nsignal_clarity," << signal_clarity << '\n';
}
