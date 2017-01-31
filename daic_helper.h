#ifndef DAIC_HELPER_H
#define DAIC_HELPER_H

#include <string>
#include <vector>

namespace daic {

/// FileToVector reads a file and converts it to a std::vector<std::string>
/// From http://www.richelbilderbeek.nl/CppFileToVector.htm
std::vector<std::string> file_to_vector(const std::string &filename);

/// Returns the path of a filename
/// From http://www.richelbilderbeek.nl/CppGetPath.htm
std::string get_path(const std::string& filename);

////Determines if a filename is a regular file
/// From http://www.richelbilderbeek.nl/CppIsRegularFile.htm
bool is_regular_file(const std::string &filename) noexcept;

std::vector<std::string> seperate_string(const std::string &input, const char seperator);

} //~namespace daic

#endif // DAIC_HELPER_H
