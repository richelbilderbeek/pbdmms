#ifndef KEWE_HELPER_H
#define KEWE_HELPER_H

#include <string>
#include <vector>

namespace kewe {

/// Examples are:
/// +-----+---------------------------+
/// | x   | gauss(x, 1.0)             |
/// +-----+---------------------------+
/// | 0.0 | e(-0.0) = 1.0             |
/// | 1.0 | e(-0.5) = 0.606530659713  |
/// | 2.0 | e(-2.0) = 0.135335283237  |
/// | 3.0 | e(-4.5) = 0.0111089965382 |
/// +-----+---------------------------+
/// @param x distance from zero
/// @param sigma standard deviation
/// @return the density of a normal distribution
double gauss(double x, double sigma) noexcept;

///Determines if a square 2D vector has only zeroes at its diagonal
bool has_diagonal_of_zeroes(const std::vector<std::vector<double>>& v);

///FileToVector reads a file and converts it to a std::vector<std::string>
///From http://www.richelbilderbeek.nl/CppFileToVector.htm
std::vector<std::string> file_to_vector(const std::string& filename);

////Determines if a filename is a regular file
///From http://www.richelbilderbeek.nl/CppIsRegularFile.htm
bool is_regular_file(const std::string& filename) noexcept;

///Determines if a 2D vector is square
bool is_square(const std::vector<std::vector<double>>& v);

///Get the maximum value of a, b and c
int get_max(const int a, const int b, const int c);

///Seperates a string by a character seperator
///From http://www.richelbilderbeek.nl/CppSeperateString.htm
std::vector<std::string> seperate_string(
  const std::string& input,
  const char seperator
);

} //~namespace kewe

#endif // KEWE_HELPER_H
