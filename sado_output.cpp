#include "sado_output.h"

#include "sado_helper.h"
#include "sado_parameters.h"
#include "sado_results.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

void sado::append_histogram(const histogram &p, const std::string &filename)
{
  assert(!p.empty());
  const double m{*std::max_element(std::begin(p), std::end(p))};
  assert(m != 0.0);

  std::stringstream s;
  for (double d : p)
  {
    s << (d / m) << ',';
  }
  std::string t{s.str()};
  assert(!t.empty());
  t.resize(t.size() - 1);

  std::ofstream f(filename, std::ios_base::app);
  f << t << '\n';
}

void sado::output(
    const population &pop, const int t, const parameters &p, results &r)
{
  const int pop_size{static_cast<int>(pop.size())};
  double ssxx = 0.0, ssxp = 0.0, sspp = 0.0, ssxq = 0.0, ssqq = 0.0, sspq = 0.0;
  const double avgx{get_mean_x(pop)};
  const double avgp{get_mean_p(pop)};
  const double avgq{get_mean_q(pop)};
  for (const auto &i : pop.get_population())
  {
    const double dxi{i.get_x() - avgx};
    const double dpi{i.get_p() - avgp};
    const double dqi{i.get_q() - avgq};
    ssxx += dxi * dxi;
    ssxp += dxi * dpi;
    ssxq += dxi * dqi;
    sspp += dpi * dpi;
    sspq += dpi * dqi;
    ssqq += dqi * dqi;
  }
  const double rhoxp{ssxp / std::sqrt(ssxx * sspp)};
  const double rhoxq{ssxq / std::sqrt(ssxx * ssqq)};
  const double rhopq{sspq / std::sqrt(sspp * ssqq)};
  const double sx{std::sqrt(ssxx / (pop_size - 1.0))};
  const double sp{std::sqrt(sspp / (pop_size - 1.0))};
  const double sq{std::sqrt(ssqq / (pop_size - 1.0))};

  const histogram histp{rescale_max_to_one(create_histogram_p(pop, p))};
  const histogram histq{rescale_max_to_one(create_histogram_q(pop, p))};
  const histogram histx{rescale_max_to_one(create_histogram_x(pop, p))};

  std::stringstream s;
  std::cout << t << ' ' << pop_size << ' ' << rhoxp << ' ' << rhoxq << ' '
            << rhopq << '\n'
            << avgx << ' ' << avgp << ' ' << avgq << ' ' << sx << ' ' << sp
            << ' ' << sq << '\n';

  {
    result this_result;
    this_result.m_histx = histx;
    this_result.m_histp = histp;
    this_result.m_histq = histq;
    this_result.m_rhopq = rhopq;
    this_result.m_rhoxp = rhoxp;
    this_result.m_rhoxq = rhoxq;
    this_result.m_sp = sp;
    this_result.m_sq = sq;
    this_result.m_sx = sx;
    this_result.m_t = t;
    this_result.m_pop_size = pop_size;
    r.add_result(this_result);
    append_histogram(histx, "eco_traits.csv");
    append_histogram(histp, "fem_prefs.csv");
    append_histogram(histq, "male_traits.csv");
    s << this_result;

    // Append to file
    {
      std::ofstream out(p.get_output_filename(), std::ios_base::app);
      out << this_result << '\n';
    }
  }

  if (is_golden_standard(p))
  {
    try
    {
      const std::string golden{get_golden_output().at((t / 10) + 1)};
      const std::string measured{s.str()};
      const histogram golden_values{to_doubles(seperate_string(golden, ','))};
      const histogram measured_values{
          to_doubles(seperate_string(measured, ','))};
      std::clog << "Comparing:\n"
                << "golden  : " << golden << '\n'
                << "measured: " << measured << '\n';
      assert(is_more_or_less_same(golden_values, measured_values));
    }
    catch (std::exception &)
    {
      // OK, is beyond golden output
    }
  }
}
