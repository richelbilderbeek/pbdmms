#ifndef ELLY_ANALYSE_H
#define ELLY_ANALYSE_H

#include <string>
#include <vector>

#include "daic_output.h"
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
std::string analyse(
  const std::string& filename);

parameters extract_parameters(const std::string& filename);

///Ideal, reality
std::vector<daic::output> extract_daic_outputs(const std::string& filename);

bool has_failed(const std::string& filename);

daic::output read_daic_output(const std::string& s);

///Removes all [a-zA-Z]
std::string remove_chars(std::string s);

///Convert the parameters to a single line
std::string to_single_line(const parameters& p, const std::string& sep = ",");

///Replaces the newlines by seperator
std::string to_single_line(const std::string& s, const std::string& sep = ",");

std::string to_single_line(const std::vector<daic::output>& p);

std::string to_single_line(const daic::output& p);

} //~namespace elly


#endif // ELLY_ANALYSE_H
