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

///the whitespace is tab
std::string daic::create_input_header() noexcept
{
  return "Clade_name\tStatus\tMissing_species\tBranching_times";
}

std::ostream& daic::operator<<(std::ostream& os, const input_row& r) noexcept
{
  std::vector<double> v = r.m_branching_times;
  os << r.m_clade_name << '\t' << r.m_status << '\t' << r.m_n_missing_species << '\t';

  //adding all branching times to stringstream, and removing the last comma
  std::stringstream s;
  for(double branching_time: v)
    {
      s << branching_time << ',';
    }
  std::string t{s.str()};
  assert(!t.empty());
  t.resize(t.size() - 1);

  os << t;
  return os;
}
