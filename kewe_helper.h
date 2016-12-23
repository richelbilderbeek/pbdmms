#ifndef KEWE_HELPER_H
#define KEWE_HELPER_H

#include <string>
#include <vector>

namespace kewe {

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

double gauss(const double x, const double sd) noexcept;

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
