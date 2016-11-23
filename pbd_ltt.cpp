#include "pbd_ltt.h"

#include <cassert>
#include <iostream>
#include "pbd_helper.h"

pbd::ltt::ltt(const int exp_n_measurements)
  : m_data{}
{
  m_data.reserve(exp_n_measurements);
}

void pbd::ltt::add_timepoint(const double t, const int n_lineages)
{
  assert(t >= 0.0);
  assert(n_lineages >= 0);
  m_data.push_back(std::make_pair(t, n_lineages));
}

pbd::ltt pbd::create_test_ltt() noexcept
{
  ltt p;
  p.add_timepoint(0.0, 2);
  p.add_timepoint(1.0, 3);
  p.add_timepoint(2.0, 4);
  p.add_timepoint(3.0, 5);
  p.add_timepoint(4.0, 6);
  return p;
}

pbd::ltt pbd::load_ltt_from_csv(const std::string& csv_filename)
{
  const std::vector<std::string> text{
    remove_first(
      file_to_vector(csv_filename)
    )
  };

  ltt my_nltt(text.size());
  if (text.size() <= 1) return my_nltt;

  for (const std::string& line: text)
  {
    //In R, there may be an extra column indivicating the row number
    //delete that thing here
    auto v = seperate_string(line, ',');
    if (v.size() == 3) { v = remove_first(v); }

    assert(v.size() == 2);
    const double t{std::stod(v[0])};
    const int n{std::stoi(v[1])}; //Number of lineages is integer
    my_nltt.add_timepoint(t, n);
  }
  return my_nltt;
}

std::ostream& pbd::operator<<(std::ostream& os, const ltt& l) noexcept
{
  os << "time,number_of_lineages\n";
  for (const auto p: l.get())
  {
    os << p.first << ',' << p.second << '\n';
  }
  return os;
}

bool pbd::operator==(const ltt& lhs, const ltt& rhs) noexcept
{
  return lhs.get() == rhs.get();
}
