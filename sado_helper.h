#ifndef SADO_HELPER_H
#define SADO_HELPER_H

#include <string>
#include <vector>

namespace sado {

std::vector<double> to_doubles(
  const std::vector<std::string>& v
);

double gauss(double xx, double sigma);


///Get the summed version, for example
///   {0.1, 0.2, 0.3} will become {0.1, 0.1 + 0.2, 0.1 + 0.2 + 0.3} which equals { 0.1, 0.3, 0.6 }
std::vector<double> get_summed(const std::vector<double>& v) noexcept;

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
