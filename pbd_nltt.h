#ifndef PBD_NLTT_H
#define PBD_NLTT_H

#include <iosfwd>
#include <string>
#include <vector>
#include "pbd_ltt.h"


namespace pbd {

using nltt = std::vector<std::pair<double, double>>;

pbd::nltt convert_to_nltt(const ltt& lineages_through_time);

nltt create_test_nltt() noexcept;

struct ltt;

///Calculates the nLTT statistic between two nLTT plots
double calc_nltt_statistic(const nltt& a, const nltt& b);

///Normalize an LTT plot
nltt convert_to_nltt(const ltt&);

///Create a rather complex nLTT
nltt create_test_nltt_1() noexcept;

///Create a simple nLTT. Datapoints created are marked 'X'
///
/// 1.0 +   X---X
///     |   |
/// 0.5 X---+
///     |
///     +---+---+
///        0.5 1.0
nltt create_test_nltt_2() noexcept;

///Create a simple nLTT. Datapoints created are marked 'X'
///
/// 1.0  +       X
/// 0.75 |   X---+
/// 0.5  |   |
/// 0.25 X---+
///      +---+---+
///        0.5 1.0
///
nltt create_test_nltt_3() noexcept;

nltt load_nltt_from_csv(const std::string& csv_filename);

std::ostream& operator<<(std::ostream& os, const nltt& p) noexcept;

} //~namespace pbd

#endif // PBD_NLTT_H
