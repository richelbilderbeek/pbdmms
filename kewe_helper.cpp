#include "kewe_helper.h"

#include <cassert>
#include <fstream>
#include <boost/algorithm/string/split.hpp>


///FileToVector reads a file and converts it to a std::vector<std::string>
///From http://www.richelbilderbeek.nl/CppFileToVector.htm
std::vector<std::string> kewe::file_to_vector(const std::string& filename)
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

bool kewe::has_diagonal_of_zeroes(const std::vector<std::vector<double>>& v)
{
  assert(is_square(v));
  const int sz{static_cast<int>(v.size())};
  for (int i=0; i!=sz; ++i)
  {
    if (v[i][i] != 0.0) return false;
  }
  return true;
}

////Determines if a filename is a regular file
///From http://www.richelbilderbeek.nl/CppIsRegularFile.htm
bool kewe::is_regular_file(const std::string& filename) noexcept
{
  std::fstream f;
  f.open(filename.c_str(),std::ios::in);
  return f.is_open();
}

bool kewe::is_square(const std::vector<std::vector<double>>& v)
{
  assert(!v.empty());
  return v.size() == v.back().size();
}


///From http://www.richelbilderbeek.nl/CppSeperateString.htm
std::vector<std::string> kewe::seperate_string(
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
double kewe::str_to_double(const std::string& s)
{
  return std::stod(s);
}
