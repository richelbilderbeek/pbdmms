#include "daic_input_row.h"
#include <stdexcept>
#include <cassert>
#include <string>
#include <sstream>

daic::input_row::input_row(
  const std::string& clade_name,
  const daic::species_status status,
  const int n_missing_species,
  const std::vector<double>& branching_times
):
  m_clade_name{clade_name},
  m_status{status},
  m_n_missing_species{n_missing_species},
  m_branching_times{branching_times}
{
  if(m_branching_times.empty())
    throw std::invalid_argument("branching times cannot be empty");
}

std::string daic::create_input_header() noexcept
{
  //the whitespace is tab
  return "Clade_name\tStatus\tMissing_species\tBranching_times";
}

std::string daic::input_row::get_clade_name()const noexcept
{
  return m_clade_name;
}

daic::species_status daic::input_row::get_status() const noexcept
{
  return m_status;
}
int daic::input_row::get_n_missing_species() const noexcept
{
  return m_n_missing_species;
}
std::vector<double> daic::input_row::get_branching_times() const noexcept
{
  return m_branching_times;
}

std::ostream& daic::operator<<(std::ostream& os, const input_row& r) noexcept
{
  std::vector<double> v = r.m_branching_times;
  os << r.m_clade_name << '\t' << r.m_status << '\t' << r.m_n_missing_species << '\t';

  //adding all branching times to stringstream, and removing the last comma

  std::stringstream s;
  for(double branching_time_raw: v)
  {
    //If there is one branching time that is zero, set it to 0.000001,
    //Due to https://github.com/richelbilderbeek/pbdmms/issues/187
    const double branching_time{
      branching_time_raw == 0.0 ? 0.000001 : branching_time_raw
    };
    assert(branching_time > 0.0);
    s << branching_time << ',';
  }
  std::string t{s.str()};
  assert(!t.empty());
  t.resize(t.size() - 1);

  os << t;
  return os;
}
