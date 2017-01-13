#include "elly_analyse.h"

#include <cassert>
#include <iterator>
#include <fstream>

#include <boost/algorithm/string/replace.hpp>

#include "elly_parameters.h"
#include "daic_helper.h"

void elly::analyse(
  const std::vector<std::string>& filenames,
  const std::string& results_filename)
{
  std::ofstream f(results_filename);
  const std::vector<std::string> lines = analyse(filenames);
  std::copy(
    std::begin(lines),
    std::end(lines),
    std::ostream_iterator<std::string>(f, "\n")
  );
}

std::vector<std::string> elly::analyse(
  const std::vector<std::string>& filenames)
{
  std::vector<std::string> v;
  for (const auto& filename: filenames)
  {
    const std::vector<std::string> w = analyse(filename);
    std::copy(
      std::begin(w),
      std::end(w),
      std::back_inserter(v)
    );
  }
  return v;
}

std::vector<std::string> elly::analyse(
  const std::string& filename)
{
  //const std::vector<std::string> lines = daic::file_to_vector(filename);
  const auto parameters = to_single_line(extract_parameters(filename));

  const std::vector<std::string> results = {parameters};
  return results;
}

elly::parameters extract_parameters(const std::string& filename)
{
  assert(daic::is_regular_file(filename));
  std::ifstream f(filename);
  while (f.peek())
  {
    try
    {
      elly::parameters p = elly::create_parameters_set2();
      f >> p;
      return p;
    }
  }
  assert(!"Should not get here");
}

std::string elly::to_single_line(
  const parameters& p, const char sep)
{
  std::stringstream s;
  s << p;
  return to_single_line(s.str());
}

std::string elly::to_single_line(
  const std::string& s, const char sep)
{
  return boost::algorithm::replace_all_copy(s, '\n', sep);
}
