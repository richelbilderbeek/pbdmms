#ifndef PBD_NLTT_H
#define PBD_NLTT_H

#include <iosfwd>
#include <string>
#include <vector>
#include "pbd_ltt.h"


namespace pbd {

class nltt
{
public:
  ///@param exp_n_measurements the expected number of measurements,
  /// just improves the speed of this class
  nltt(const int exp_n_measurements = 0);

  /// @param norm_t normalized time
  /// @param norm_n_lineages normalized number of lineages
  void add_timepoint(const double norm_t, const double norm_n_lineages);

  bool empty() const noexcept { return m_data.empty(); }

  const auto& get() const noexcept { return m_data; }

  int size() const noexcept { return static_cast<int>(m_data.size()); }

  private:
  ///A pair consists out of {timepoint, number of lineages}
  std::vector<std::pair<double, double>> m_data;
};

pbd::nltt convert_to_nltt(const ltt& lineages_through_t);

nltt create_test_nltt() noexcept;

struct ltt;

///Calculates the nLTT statistic between two nLTT plots
double calc_nltt_statistic(const nltt& a, const nltt& b);

///Collect the normalized lineages of an nLTT
std::vector<double> collect_nls(const nltt& n) noexcept;

///Collect the normalized times of an nLTT
std::vector<double> collect_nts(const nltt& n) noexcept;

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
