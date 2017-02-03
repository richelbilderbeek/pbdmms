#include "pbd_nltt.h"
#include "pbd_ltt.h"
#include <algorithm>
#include <cassert>
#include <iostream>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include "pbd_ltt.h"
#include "pbd_helper.h"

pbd::nltt::nltt(const int exp_n_measurements)
  : m_data{}
{
  m_data.reserve(exp_n_measurements);
}

void pbd::nltt::add_timepoint(const double norm_t, const double norm_n_lineages)
{
  assert(norm_t >= 0.0);
  assert(norm_t <= 1.0);
  assert(norm_n_lineages >= 0.0);
  assert(norm_n_lineages <= 1.0);
  assert(m_data.empty() || m_data.back().first <= norm_t);
  m_data.push_back(std::make_pair(norm_t, norm_n_lineages));
}

pbd::nltt pbd::calc_average(const std::vector<nltt>& nltts)
{
  const auto nts = collect_nts(nltts);
  assert(std::is_sorted(std::begin(nts), std::end(nts)));
  nltt n;
  n.reserve(nts.size());
  for (const double nt: nts)
  {
    n.add_timepoint(nt, calc_average_nls(nt, nltts));
  }
  return n;
}

double pbd::calc_average_nls(
  const double nt,
  const std::vector<nltt>& nltts)
{
  return std::accumulate(
    std::begin(nltts),
    std::end(nltts),
    0.0,
    [nt](const double init, const nltt& n)
    {
      return init + n.get_n(nt);
    }
  ) / static_cast<double>(nltts.size());
}

double pbd::calc_nltt_statistic(const nltt& a, const nltt& b)
{
  const std::vector<double> nts = collect_nts(a, b);
  if (nts.size() <= 1)
  {
    throw std::invalid_argument("Need two timepoints to calculate nLTT statistic");
  }
  const int sz{static_cast<int>(nts.size())};
  double error{0.0};
  for (int i=0; i!=sz-1; ++i)
  {
    const double t{nts[i+0]};
    const double u{nts[i+1]};
    const double dt{u - t};
    const double ta{a.get_n(t)};
    const double tb{b.get_n(t)};
    const double diff{std::abs(ta - tb)};
    const double area{dt * diff};
    error += area;
  }

  return error;
}

///Converts a two nLtt saved file,
///to a double with the differences between the nLTT's
double pbd::get_nltt_statistic(const std::string& csv_filename_1,
                          const std::string& csv_filename_2)
{
  nltt nltt_1 = load_nltt_from_csv(csv_filename_1);
  nltt nltt_2 = load_nltt_from_csv(csv_filename_2);
  return calc_nltt_statistic(nltt_1,nltt_2);
}

std::vector<double> pbd::collect_nls(const nltt& n) noexcept
{
  const auto& ps = n.get();
  std::vector<double> ts;
  ts.reserve(ps.size());
  std::transform(
    std::begin(ps),
    std::end(ps),
    std::back_inserter(ts),
    [](const auto& p)
    {
      return p.second;
    }
  );
  return ts;
}

std::vector<double> pbd::collect_nts(const nltt& n) noexcept
{
  const auto& ps = n.get();
  std::vector<double> ts;
  ts.reserve(ps.size());
  std::transform(
    std::begin(ps),
    std::end(ps),
    std::back_inserter(ts),
    [](const auto& p)
    {
      return p.first;
    }
  );
  return ts;
}

std::vector<double> pbd::collect_nts(const nltt& a, const nltt& b) noexcept
{
  std::vector<double> v = collect_nts(a);
  {
    const std::vector<double> w = collect_nts(b);
    std::copy(std::begin(w), std::end(w), std::back_inserter(v));
  }
  std::sort(std::begin(v), std::end(v));
  const auto new_end = std::unique(std::begin(v), std::end(v));
  v.erase(new_end, std::end(v));
  return v;
}

std::vector<double> pbd::collect_nts(const std::vector<nltt>& nltts) noexcept
{
  std::vector<double> v;
  for (const auto& n: nltts)
  {
    const std::vector<double> w = collect_nts(n);
    std::copy(std::begin(w), std::end(w), std::back_inserter(v));
  }
  std::sort(std::begin(v), std::end(v));
  const auto new_end = std::unique(std::begin(v), std::end(v));
  v.erase(new_end, std::end(v));
  return v;
}


pbd::nltt pbd::create_test_nltt_1() noexcept
{
  nltt p;
  p.add_timepoint(0.0, 0.1);
  p.add_timepoint(0.2, 0.3);
  p.add_timepoint(0.4, 0.4);
  p.add_timepoint(0.8, 0.7);
  p.add_timepoint(1.0, 1.0);
  return p;
}

pbd::nltt pbd::create_test_nltt_2() noexcept
{
  nltt p;
  p.add_timepoint(0.0, 0.5);
  p.add_timepoint(0.5, 1.0);
  p.add_timepoint(1.0, 1.0);
  return p;
}

pbd::nltt pbd::create_test_nltt_3() noexcept
{
  nltt p;
  p.add_timepoint(0.0, 0.25);
  p.add_timepoint(0.5, 0.75);
  p.add_timepoint(1.0, 1.0);
  return p;
}

double pbd::nltt::get_n(const double t) const
{
  if (t < 0.0 || t > 1.0)
  {
    throw std::invalid_argument(
      "(normalized) t must be in range [0,1]");
  }
  assert(std::is_sorted(std::begin(m_data), std::end(m_data)));
  auto iter = std::begin(m_data);
  const auto end = std::end(m_data);
  double n = iter->second; //Normalized number of lineages
  while (1)
  {
    ++iter;
    if (iter == end || iter->first > t) break;
    n = iter->second;
  }
  return n;
}

pbd::nltt pbd::load_nltt_from_csv(const std::string& csv_filename)
{
  if (!is_regular_file(csv_filename))
  {
    throw std::invalid_argument("Cannot load nLTT from absent file");
  }
  const std::vector<std::string> text{
    remove_first(
      file_to_vector(csv_filename)
    )
  };

  nltt my_nltt(text.size());
  if (text.size() <= 1) return my_nltt;

  for (const std::string& line: text)
  {
    //In R, there may be an extra column indivicating the row number
    //delete that thing here
    auto v = seperate_string(line, ',');
    if (v.size() == 3) { v = remove_first(v); }

    assert(v.size() == 2);
    const double t{std::stod(v[0])};
    const double n{std::stod(v[1])};
    my_nltt.add_timepoint(t, n);
  }
  return my_nltt;
}

std::vector<pbd::nltt> pbd::load_nltts_from_csvs(
  const std::vector<std::string>& csv_filenames)
{
  std::vector<nltt> v;
  v.reserve(csv_filenames.size());
  std::transform(
    std::begin(csv_filenames),
    std::end(csv_filenames),
    std::back_inserter(v),
    [](const std::string& csv_filename)
    {
      return load_nltt_from_csv(csv_filename);
    }
  );
  return v;

}

pbd::nltt pbd::convert_to_nltt(const ltt& lineages_through_t)
{
  const int max_n_lineages_algo{
    (*std::max_element(
      std::begin(lineages_through_t.get()),
      std::end(lineages_through_t.get()),
      [](const auto& lhs, const auto rhs)
      {
        return lhs.second < rhs.second;
      }
    )).second
  };
  const double max_timepoint {lineages_through_t.get().back().first};
  const int ltt_sz{lineages_through_t.size()};
  //std::cout << ltt_sz << '\n';

  const auto& v = lineages_through_t.get();
  pbd::nltt new_nltt;
  // To get first values for nltt, normalized timepoint = 0,
  // normalized number of lineages is 1/max_n_lineages
  new_nltt.add_timepoint(
    0.0,
    static_cast<double>(v.front().second) / static_cast<double>(max_n_lineages_algo)
  );

  for (int i=1; i!=ltt_sz; ++i)
  {
    if (v[i].first > new_nltt.get().back().first)
    {
      // To get next values for nltt, if number of lineages is bigger than previous,
      // add n_lineages/max_n_lineages and timepoint/max_timepoint
      new_nltt.add_timepoint(
        static_cast<double>(v[i].first )/static_cast<double>(max_timepoint ),
        static_cast<double>(v[i].second)/static_cast<double>(max_n_lineages_algo)
      );
    }
  }
  return new_nltt;
}

std::ostream& pbd::operator<<(std::ostream& os, const nltt& l) noexcept
{
  os << "normalized_time,normalized_number_of_lineages\n";
  for (const auto p: l.get())
  {
    os << p.first << ',' << p.second << '\n';
  }
  return os;
}

