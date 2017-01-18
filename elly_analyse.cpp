#include "elly_analyse.h"

#include <cassert>
#include <iterator>
#include <fstream>
#include <stdexcept>
#include <sstream>

#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/erase.hpp>

#include "elly_parameters.h"
#include "daic_helper.h"

void elly::analyse(
  const std::vector<std::string>& filenames,
  const std::string& results_filename)
{
  std::stringstream s;
  s
    << elly::get_parameters_heading() << ','
    << daic::get_output_header() << ','
    << daic::get_output_header() << '\n'
  ;
  const std::vector<std::string> lines = analyse(filenames);
  std::copy(
    std::begin(lines),
    std::end(lines),
    std::ostream_iterator<std::string>(s, "\n")
  );

  std::ofstream f(results_filename);
  f << s.str();

}

std::vector<std::string> elly::analyse(
  const std::vector<std::string>& filenames)
{
  std::vector<std::string> v;
  std::string header{
      elly::get_parameters_heading()
    + daic::get_output_header() + ","
    + daic::get_output_header()
  };
  boost::replace_all(header, "\"", "");
  v.push_back(header);
  for (const auto& filename: filenames)
  {
    v.push_back(analyse(filename));
  }
  return v;
}

std::string elly::analyse(
  const std::string& filename)
{
  const auto parameters = remove_chars(
    to_single_line(extract_parameters(filename))
  );
  std::string results;
  try
  {
    results = to_single_line(extract_daic_outputs(filename));
  }
  catch (std::runtime_error&)
  {
    results = "NA";
  }

  return parameters + results;
}

std::vector<daic::output> elly::extract_daic_outputs(const std::string& filename)
{
  assert(daic::is_regular_file(filename));
  std::vector<daic::output> p;
  const auto lines = daic::file_to_vector(filename);
  for (const auto& line: lines)
  {
    try
    {
      p.push_back(read_daic_output(line));
    }
    catch (std::invalid_argument&)
    { //!OCLINT
      //OK
    }
  }
  if (p.size() != 2)
  {
    throw std::runtime_error("Did not find two DAISIE outputs");
  }
  return p;
}

elly::parameters elly::extract_parameters(const std::string& filename)
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
    catch (std::invalid_argument&)
    { //!OCLINT
      //OK
    }
  }
  assert(!"Should not get here"); //!OCLINT accepted idiom
}

daic::output elly::read_daic_output(const std::string& s)
{
  const std::vector<std::string> v = daic::seperate_string(s, '\t');
  if (v.size() != 8)
  {
    throw std::invalid_argument("Invalid DAISIE output");
  }
  return daic::output(
    std::stod(v[0]),
    std::stod(v[1]),
    std::stod(v[2]),
    std::stod(v[3]),
    std::stod(v[4]),
    std::stod(v[5]),
    std::stoi(v[6]),
    std::stoi(v[7])
  );
}

std::string elly::remove_chars(std::string s)
{
  const std::string to_erase{
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_: "
  };
  for (const char c: to_erase)
  {
    std::string t;
    t.push_back(c);
    boost::algorithm::erase_all(s, t);
  }
  return s;
}

std::string elly::to_single_line(
  const parameters& p, const std::string& sep)
{
  std::stringstream s;
  s << p;
  return to_single_line(s.str(), sep);
}

std::string elly::to_single_line(
  const std::string& s, const std::string& sep)
{
  return boost::algorithm::replace_all_copy(s, "\n", sep);
}

std::string elly::to_single_line(const std::vector<daic::output>& p)
{
  assert(p.size() == 2);
  return to_single_line(p[0]) + std::string(",") + to_single_line(p[1]);
}

std::string elly::to_single_line(const daic::output& p)
{
  std::stringstream s;
  s << p;
  std::string t = s.str();
  return boost::algorithm::replace_all_copy(t, "\t", ",");
}
