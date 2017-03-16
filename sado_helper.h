#ifndef SADO_HELPER_H
#define SADO_HELPER_H

#include <string>
#include <vector>

namespace sado
{

/// Deletes a file
void delete_file(const std::string& filename);

/// Deletes a file only if it is present
void delete_file_if_present(const std::string& filename);


/// FileToVector reads a file and converts it to a std::vector<std::string>
/// From http://www.richelbilderbeek.nl/CppFileToVector.htm
std::vector<std::string> file_to_vector(const std::string &filename);

/// Return x that is at least min and max at most
int get_constrained(int x, const int min, const int max);

/// Get the summed version, for example
///   {0.1, 0.2, 0.3} will become {0.1, 0.1 + 0.2, 0.1 + 0.2 + 0.3} which equals
///   { 0.1, 0.3, 0.6 }
std::vector<double> get_summed(const std::vector<double> &v) noexcept;

/// Get the (sorted and) unique values in v
std::vector<int> get_unique(std::vector<int> v);

///Determines if a square 2D vector has only zeroes at its diagonal
bool has_diagonal_of_zeroes(const std::vector<std::vector<double>>& v);

/// Optimistic comparison
bool is_more_or_less_same(
    const std::vector<double> &v, const std::vector<double> &w);

///Determines if a 2D vector is square
bool is_square(const std::vector<std::vector<double>>& v);

////Determines if a filename is a regular file
/// From http://www.richelbilderbeek.nl/CppIsRegularFile.htm
bool is_regular_file(const std::string &filename) noexcept;

///Detects if this code is run on a Travis CI server
bool is_travis() noexcept;

/// Draw a random number from 0 to pop_size
int pick_random_individual_index(const int pop_size);

std::vector<std::string>
seperate_string(const std::string &input, const char seperator);

///Ensures the square 2D vector will have only zeroes at its diagonal
void set_diagonal_of_zeroes(std::vector<std::vector<double>>& v);

std::vector<double> to_doubles(const std::vector<std::string> &v);

} //~namespace sado

#endif // SADO_HELPER_H
