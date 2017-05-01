#ifndef JAAN_HABITAT_H
#define JAAN_HABITAT_H
#include <fstream>

class Habitat
{
public:
    Habitat(
            double init_optimal_pref,
            double init_optimal_quality,
            double init_optimal_trait,
            double init_selection_on_pref,
            double init_selection_on_qual,
            double init_selection_on_trt,
            double init_expr_efficiency);
    Habitat(const Habitat& other);
    double get_optimal_preference() const;
    double get_optimal_quality() const;
    double get_optimal_trait() const;
    double get_selection_on_pref() const;
    double get_selection_on_qual() const;
    double get_selection_on_trt() const;
    double get_expr_efficiency() const;
    void print_habitat(std::ofstream& output) const;
    Habitat& operator=(Habitat);
private:
    double optimal_preference;    /// Optimal preference value for viability.
    double optimal_quality;       /// Optimal quality value for viability.
    double optimal_trait;         /// Optimal trait value for viability.
    double selection_on_pref;     /// Higher = Lower cost of choosiness.
    double selection_on_qual;     /// Effect of quality on viability.
    double selection_on_trt;      /// Higher = Lower cost of trait.
    /// selection_on_trt << selection_on_pref.
    double expr_efficiency;       /// Effect of quality on attractiveness.
};

#endif // JAAN_HABITAT_H
