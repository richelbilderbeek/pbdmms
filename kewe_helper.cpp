#include "kewe_helper.h"

#include <cassert>
#include <cmath>
#include <fstream>
#include <boost/algorithm/string/split.hpp>


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

double kewe::gauss(double x, double sigma) noexcept
{
  return std::exp(
    - (x*x)
    / (2.0*sigma*sigma)
  );
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


