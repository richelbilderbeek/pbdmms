#include <cassert>
#include <fstream>
#include <vector>
#include <algorithm>
#include <boost/algorithm/string/split.hpp>
#include "kewe_parameters.h"
#include "kewe_SES.h"

///Determines if a filename is a regular file
///From http://www.richelbilderbeek.nl/CppIsRegularFile.htm
bool is_regular_file(const std::string& filename) noexcept
{
  std::fstream f;
  f.open(filename.c_str(),std::ios::in);
  return f.is_open();
}

///FileToVector reads a file and converts it to a std::vector<std::string>
///From http://www.richelbilderbeek.nl/CppFileToVector.htm
std::vector<std::string> file_to_vector(const std::string& filename)
{
  assert(is_regular_file(filename));
  std::vector<std::string> v;
  std::ifstream in(filename.c_str());
  for (int i=0; !in.eof(); ++i)
  {
    std::string s;
    std::getline(in,s);
    v.push_back(s);
  }
  return v;
}

///From http://www.richelbilderbeek.nl/CppSeperateString.htm
std::vector<std::string> seperate_string(
  const std::string& input,
  const char seperator)
{
  std::vector<std::string> v;
  boost::algorithm::split(v,input,
  std::bind2nd(std::equal_to<char>(),seperator),
  boost::algorithm::token_compress_on);
  return v;
}


//From http://www.richelbilderbeek.nl/CppStrToDouble.htm
double str_to_double(const std::string& s)
{
  return std::stof(s);
}

kewe_parameters read_parameters(const std::string& filename)
{
  const auto lines = file_to_vector(filename);

  kewe_parameters parameters;

  for (const std::string& line: lines)
  {
    std::vector<std::string> v{seperate_string(line, ' ')};
    if (v[0] == "alleles")
      {
        v.erase(v.begin());
        for (int i = 0; i < static_cast<int>(v.size()); ++i)
        {
          switch(i)
          {
            case 0: parameters.sim_parameters.Nx = str_to_double(v[0]); break;
            case 1: parameters.sim_parameters.Np = str_to_double(v[1]); break;
            case 2: parameters.sim_parameters.Nq = str_to_double(v[2]); break;
            default: throw std::invalid_argument("Too many parameters after \"alleles\"");
          }
        }
      }
  }

  ///TODO

  return parameters;
}

void create_test_parameter_file(const std::string& filename)
{
  std::ofstream f(filename.c_str());
  f << "STUB";
}
