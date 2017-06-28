#include "jaan_habitat.h"

Habitat::Habitat(
        double init_optimal_pref,
        double init_optimal_qual,
        double init_optimal_trait,
        double init_selection_on_pref,
        double init_selection_on_qual,
        double init_selection_on_trt,
        double init_expr_efficiency):
    optimal_preference(init_optimal_pref),
    optimal_quality(init_optimal_qual),
    optimal_trait(init_optimal_trait),
    selection_on_pref(init_selection_on_pref),
    selection_on_qual(init_selection_on_qual),
    selection_on_trt(init_selection_on_trt),
    expr_efficiency(init_expr_efficiency)
{
}

Habitat::Habitat(const Habitat& other):
    optimal_preference(other.get_optimal_preference()),
    optimal_quality(other.get_optimal_quality()),
    optimal_trait(other.get_optimal_trait()),
    selection_on_pref(other.get_selection_on_pref()),
    selection_on_qual(other.get_selection_on_qual()),
    selection_on_trt(other.get_selection_on_trt()),
    expr_efficiency(other.get_expr_efficiency())
{
}

double Habitat::get_optimal_preference() const
{
    return optimal_preference;
}

double Habitat::get_optimal_quality() const
{
    return optimal_quality;
}

double Habitat::get_optimal_trait() const
{
    return optimal_trait;
}

double Habitat::get_selection_on_pref() const
{
    return selection_on_pref;
}

double Habitat::get_selection_on_qual() const
{
    return selection_on_qual;
}

double Habitat::get_selection_on_trt() const
{
    return selection_on_trt;
}

double Habitat::get_expr_efficiency() const
{
    return expr_efficiency;
}

/// Prints the parameters of the simulation to the output file.
void Habitat::print_habitat(std::ofstream& output) const
{
    output << "optimal_preference," << optimal_preference
           << "\noptimal_quality," << optimal_quality
           << "\noptimal_trait," << optimal_trait
           << "\nselection_on_pref," << selection_on_pref
           << "\nselection_on_quality," << selection_on_qual
           << "\nselection_on_trt," << selection_on_trt
           << "\nexpr_efficiency," << expr_efficiency<< std::endl;
}

Habitat& Habitat::operator=(Habitat other)
{
    std::swap(optimal_preference, other.optimal_preference);
    std::swap(optimal_quality, other.optimal_quality);
    std::swap(optimal_trait, other.optimal_trait);
    std::swap(selection_on_pref, other.selection_on_trt);
    std::swap(selection_on_qual, other.selection_on_qual);
    std::swap(selection_on_trt, other.selection_on_trt);
    std::swap(expr_efficiency, other.expr_efficiency);
    return *this;
}
