#include "sado_population.h"

#include <cassert>
#include <numeric>

double sado::get_mean_x(const population &p) {
  assert(!p.empty());
  return std::accumulate(std::begin(p), std::end(p), 0.0,
                         [](const double init, const indiv &i) {
                           return init + i.get_x();
                         }) /
         static_cast<double>(p.size());
}

double sado::get_mean_p(const population &p) {
  assert(!p.empty());
  return std::accumulate(std::begin(p), std::end(p), 0.0,
                         [](const double init, const indiv &i) {
                           return init + i.get_p();
                         }) /
         static_cast<double>(p.size());
}

double sado::get_mean_q(const population &p) {
  assert(!p.empty());
  return std::accumulate(std::begin(p), std::end(p), 0.0,
                         [](const double init, const indiv &i) {
                           return init + i.get_q();
                         }) /
         static_cast<double>(p.size());
}
