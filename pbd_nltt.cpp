#include "pbd_nltt.h"
#include "pbd_ltt.h"
#include <cassert>
#include <iostream>
#include "pbd_helper.h"

double pbd::calc_nltt_statistic(const nltt& /* a */, const nltt& /* b */)
{
  return 0.0;
}

//TODO Make function work
pbd::nltt pbd::convert_to_nltt(const ltt& /*lineages_through_time*/)
{

  // you get ltt = std::vector<std::pair<double, int>;
  // with timepoint as double and the number of lineages as int
  // Check maximum time and n_lineages
  std::vector<std::pair<double, int>> lineages_through_time;
  auto x = ltt.get();
  int max_n_lineages = ltt.get().back.first();
  double max_timestep = (ltt.back().first);
  const int sz{static_cast<int>(ltt.size())};
  // Make nltt vector
  nltt = new_nltt;
  // Push back start values of nltt vector
  new_nltt.push_back(std::make_pair((ltt.front.first()/max_n_lineages), 0);
  // for-loop to run through number of lineages untill we get more lineages
  for (int t=0; t!=sz; ++t)
  {
    // if a number of lineages is higher than previous number
    if(ltt[t].second > new_nltt.back.second)
    {
      double norm_timepoint = ltt[t].second()/max_timestep;
      double norm_lineages = ltt[t].first()/max_n_lineages;
      new_nltt.push_back(std::make_pair(norm_timepoint,norm_lineages));
    }
  }

  // first point of the nltt needs to be 1/max number of lineages at 0
  // next point of the nltt needs to be 2/max number of lineages at timestep/max time

  // you want nllt = std::vector<std::pair<double, double>;
  return nltt();
}


pbd::nltt pbd::create_test_nltt_1() noexcept
{
  nltt p;
  p.push_back(std::make_pair(0.0, 0.1));
  p.push_back(std::make_pair(0.0, 0.3));
  p.push_back(std::make_pair(0.0, 0.4));
  p.push_back(std::make_pair(0.0, 0.5));
  p.push_back(std::make_pair(1.0, 1.0));
  return p;
}

pbd::nltt pbd::create_test_nltt_2() noexcept
{
  nltt p;
  p.push_back(std::make_pair(0.0, 0.5));
  p.push_back(std::make_pair(0.5, 1.0));
  p.push_back(std::make_pair(1.0, 1.0));
  return p;
}

pbd::nltt pbd::create_test_nltt_3() noexcept
{
  nltt p;
  p.push_back(std::make_pair(0.0, 0.25));
  p.push_back(std::make_pair(0.5, 0.75));
  p.push_back(std::make_pair(1.0, 1.0));
  return p;
}

pbd::nltt pbd::load_nltt_from_csv(const std::string& csv_filename)
{
  const std::vector<std::string> text{
    remove_first(
      file_to_vector(csv_filename)
    )
  };

  nltt my_nltt;
  if (text.size() <= 1) return my_nltt;
  my_nltt.reserve(text.size());

  for (const std::string& line: text)
  {
    //In R, there may be an extra column indivicating the row number
    //delete that thing here
    auto v = seperate_string(line, ',');
    if (v.size() == 3) { v = remove_first(v); }

    assert(v.size() == 2);
    const double t{std::stod(v[0])};
    const double n{std::stod(v[1])};
    my_nltt.push_back(std::make_pair(t, n));
  }
  return my_nltt;
}

std::ostream& pbd::operator<<(std::ostream& os, const nltt& l) noexcept
{
  os << "normalized_time,normalized_number_of_lineages\n";
  for (const auto p: l)
  {
    os << p.first << ',' << p.second << '\n';
  }
  return os;
}
