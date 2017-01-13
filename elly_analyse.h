#ifndef ELLY_ANALYSE_H
#define ELLY_ANALYSE_H

#include <string>
#include <vector>

#include "elly_parameters.h"

namespace elly {

///From multiple files, write the results to file
void analyse(
  const std::vector<std::string>& filenames,
  const std::string& results_filename);

///From multiple files, get the lines to write to file
std::vector<std::string> analyse(
  const std::vector<std::string>& filenames);

///From one file, get the lines to write to file
std::vector<std::string> analyse(
  const std::string& filename);

parameters extract_parameters(const std::string& filename);

///Convert the parameters to a single line
std::string to_single_line(const parameters& p, const char sep = ',');

///Replaces the newlines by seperator
std::string to_single_line(const std::string& s, const char sep = ',');

} //~namespace elly


#endif // ELLY_ANALYSE_H
