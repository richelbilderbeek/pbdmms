#ifndef SADO_HELPER_H
#define SADO_HELPER_H

#include <string>
#include <vector>

namespace sado {

///FileToVector reads a file and converts it to a std::vector<std::string>
///From http://www.richelbilderbeek.nl/CppFileToVector.htm
std::vector<std::string> file_to_vector(const std::string& filename);

///Get the summed version, for example
///   {0.1, 0.2, 0.3} will become {0.1, 0.1 + 0.2, 0.1 + 0.2 + 0.3} which equals { 0.1, 0.3, 0.6 }
std::vector<double> get_summed(const std::vector<double>& v) noexcept;

///Optimistic comparison
bool is_more_or_less_same(
  const std::vector<double>& v,
  const std::vector<double>& w
);

////Determines if a filename is a regular file
///From http://www.richelbilderbeek.nl/CppIsRegularFile.htm
bool is_regular_file(const std::string& filename) noexcept;

///Draw a random number from 0 to pop_size
int pick_random_individual_index(
  const int pop_size
);

std::vector<std::string> seperate_string(
  const std::string& input,
  const char seperator
);

std::vector<double> to_doubles(
  const std::vector<std::string>& v
);

} //~namespace sado

#endif // SADO_HELPER_H
