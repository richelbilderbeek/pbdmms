#include "sado_histogram.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <sstream>

#include "sado_helper.h"
#include "sado_parameters.h"

sado::histogram
sado::create_histogram_p(const population &pop, const parameters &params)
{
  const int histw{params.get_histw()};
  const double delta{1.0 / static_cast<double>(pop.size())};

  histogram histp(histw, 0.0);
  for (const auto &i : pop.get_population())
  {
    const double p{i.get_p()};
    const double histbinp{params.get_histbinp()};
    const int index{get_constrained(
        static_cast<int>((params.get_histw() / 2.0) + (p / histbinp)),
        0,
        histw - 1)};
    histp[index] += delta;
  }
  return histp;
}

sado::histogram
sado::create_histogram_q(const population &pop, const parameters &p)
{
  const int histw{p.get_histw()};
  const double delta{1.0 / static_cast<double>(pop.size())};

  histogram histq(histw, 0.0);
  for (const auto &i : pop.get_population())
  {
    const double q{i.get_q()};
    const double histbinq{p.get_histbinq()};
    const int index{get_constrained(
        static_cast<int>((histw / 2.0) + (q / histbinq)), 0, histw - 1)};
    histq[index] += delta;
  }
  return histq;
}

sado::histogram
sado::create_histogram_x(const population &pop, const parameters &p)
{
  const int histw{p.get_histw()};
  const double delta{1.0 / static_cast<double>(pop.size())};

  histogram histx(histw, 0.0);
  for (const auto &i : pop.get_population())
  {
    const double x{i.get_x()};
    const double histbinx{p.get_histbinx()};
    const int index{get_constrained(
        static_cast<int>((histw / 2.0) + (x / histbinx)), 0, histw - 1)};
    histx[index] += delta;
  }
  return histx;
}

sado::histogram sado::rescale_max_to_one(histogram h)
{
  const double max{*std::max_element(std::begin(h), std::end(h))};
  assert(max != 0.0);
  for (double &d : h)
  {
    d /= max;
  };
  return h;
}

std::ostream &sado::operator<<(std::ostream &os, const histogram &h) noexcept
{
  if (h.empty())
    return os;
  std::stringstream s;
  for (auto d : h)
  {
    s << d << ',';
  }
  std::string t{s.str()};
  assert(!t.empty());
  t.resize(t.size() - 1);
  os << t;
  return os;
}
