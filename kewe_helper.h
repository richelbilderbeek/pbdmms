#ifndef KEWE_HELPER_H
#define KEWE_HELPER_H

#include <string>
#include <vector>

namespace kewe {

bool has_diagonal_of_zeroes(const std::vector<std::vector<double>>& v);

std::vector<std::string> file_to_vector(const std::string& filename);

bool is_regular_file(const std::string& filename) noexcept;

bool is_square(const std::vector<std::vector<double>>& v);

std::vector<std::string> seperate_string(
  const std::string& input,
  const char seperator
);

double str_to_double(const std::string& s);

} //~namespace kewe

#endif // KEWE_HELPER_H
