#include "pbd_nltt.h"
#include "pbd_ltt.h"
#include <cassert>
#include <iostream>
#include "pbd_ltt.h"
#include "pbd_helper.h"

double pbd::calc_nltt_statistic(const nltt& /* a */, const nltt& /* b */)
{
  return 0.0;
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

pbd::nltt pbd::convert_to_nltt(const ltt& lineages_through_time)
{
 // ltt = std::vector<std::pair<double, int>>
 // with timepoint (double) and number of lineages (int)
 const int max_n_lineages {lineages_through_time.get().back().second};
 const double max_timepoint {lineages_through_time.get().back().first};
 const int ltt_sz{lineages_through_time.size()};

 // nltt = std::vector<std::pair<double, double>>
 // with normalized time (double) and normalized number of lineages (double)
 const auto& v = lineages_through_time.get();
 std::vector<std::pair<double, double>> new_nltt;
 // To get first values for nltt, normalized timepoint = 0,
 // normalized number of lineages is 1/max_n_lineages
 new_nltt.push_back(std::make_pair(0,((static_cast<double>(v.front().second))/
                                      (static_cast<double>(max_n_lineages)))));
 const int nltt_sz{static_cast<int>(new_nltt.size())};
 assert(nltt_sz==1);

 for (int i=1; i!=ltt_sz; ++i)
 {
     if ((v[i].second) > (new_nltt.back().second))
     {
     // To get next values for nltt, if number of lineages is bigger than previous,
     // add n_lineages/max_n_lineages and timepoint/max_timepoint
     new_nltt.push_back(std::make_pair(
     ((static_cast<double>(v[i].first))/static_cast<double>(max_timepoint)),
     ((static_cast<double>(v[i].second))/static_cast<double>(max_n_lineages))
     ));
     }
 }
  return new_nltt;
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
