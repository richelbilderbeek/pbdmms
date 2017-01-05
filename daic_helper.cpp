#include "daic_helper.h"
#include "sado_random.h"
#include <boost/algorithm/string/split.hpp>
#include <cmath>
#include <fstream>

std::vector<std::string> daic::file_to_vector(const std::string &filename)
{
  assert(is_regular_file(filename));
  std::vector<std::string> v;
  std::ifstream in(filename.c_str());
  for (int i = 0; !in.eof(); ++i)
  {
    std::string s;
    std::getline(in, s);
    v.push_back(s);
  }
  return v;
}

bool daic::is_regular_file(const std::string &filename) noexcept
{
  std::fstream f;
  f.open(filename.c_str(), std::ios::in);
  return f.is_open();
}

std::vector<std::string> daic::seperate_string(const std::string &input, const char seperator)
{
  std::vector<std::string> v;
  boost::algorithm::split(
      v,
      input,
      std::bind2nd(std::equal_to<char>(), seperator),
      boost::algorithm::token_compress_on);
  return v;
}

