#ifndef SADO_HELPER_H
#define SADO_HELPER_H

#include <string>
#include <vector>

namespace sado {

std::vector<double> to_doubles(
  const std::vector<std::string>& v
);

double gauss(double xx, double sigma);

std::vector<std::string> get_golden_output() noexcept;

///Optimistic comparison
bool is_more_or_less_same(
  const std::vector<double>& v,
  const std::vector<double>& w
);

std::vector<std::string> seperate_string(
  const std::string& input,
  const char seperator
);

} //~namespace sado

#endif // SADO_HELPER_H
