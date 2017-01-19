#ifndef PBD_LTT_H
#define PBD_LTT_H

#include <iosfwd>
#include <string>
#include <vector>

namespace pbd {

class ltt
{
public:
  ///@param exp_n_measurements the expected number of measurements,
  /// just improves the speed of this class
  ltt(const int exp_n_measurements = 0);

  void add_timepoint(const double t, const int n_lineages);

  bool empty() const noexcept { return m_data.empty(); }

  const auto& get() const noexcept { return m_data; }

  int size() const noexcept { return static_cast<int>(m_data.size()); }

  private:
  ///A pair consists out of {timepoint, number of lineages}
  std::vector<std::pair<double, int>> m_data;
};

ltt create_test_ltt() noexcept;
ltt create_test_ltt_2() noexcept;

///Every line in the file has one LTT value
ltt load_ltt_from_csv(const std::string& csv_filename);

///All LTTs are saved in one line, seperarated by a space
ltt load_ltt_from_csv_one_liner(const std::string& csv_filename);

std::ostream& operator<<(std::ostream& os, const ltt& p) noexcept;

bool operator==(const ltt& lhs, const ltt& rhs) noexcept;

} //~namespace pbd

#endif // PBD_LTT_H
