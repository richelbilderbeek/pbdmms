#ifndef JAAN_HABITAT_H
#define JAAN_HABITAT_H
#include <fstream>

class Habitat
{
public:
    Habitat(
            const double init_optimal_pref,
            const double init_optimal_trait,
            const double init_selection_on_pref,
            const double init_selection_on_trt,
            const double init_expr_efficiency,
            const double init_selection_on_quality);
    double get_optimal_preference() const;
    double get_optimal_trait() const;
    double get_selection_on_pref() const;
    double get_selection_on_trt() const;
    double get_expr_efficiency() const;
    double get_selection_on_quality() const;
    double get_signal_clarity() const;
    void print_habitat(std::ofstream& output) const;
private:
    const double optimal_preference;    /// Optimal preference value for viability.
    const double optimal_trait;         /// Optimal trait value for viability.
    const double selection_on_pref;     /// Higher = Lower cost of choosiness.
    const double selection_on_trt;      /// Higher = Lower cost of trait.
    /// selection_on_trt << selection_on_pref.
    const double expr_efficiency;       /// Effect of quality on attractiveness.
    const double selection_on_quality;  /// Effect of quality on viability.
    const double signal_clarity;        /// Clarity of the trait in the environment.
};

#endif // JAAN_HABITAT_H
