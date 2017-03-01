#include "sado_helper.h"

#include "sado_random.h"
#include <boost/algorithm/string/split.hpp>
#include <cmath>
#include <fstream>

std::vector<std::string> sado::file_to_vector(const std::string &filename)
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

int sado::get_constrained(int x, const int min, const int max)
{
  assert(min <= max);
  if (x < min)
    return min;
  if (x > max)
    return max;
  return x;
}

std::vector<double> sado::get_summed(const std::vector<double> &v) noexcept
{
  std::vector<double> w;
  w.reserve(v.size());

  double sum{0.0};

  for (const double d : v)
  {
    sum += d;
    w.push_back(sum);
  }
  return w;
}

bool sado::has_diagonal_of_zeroes(const std::vector<std::vector<double>>& v)
{
  assert(is_square(v));
  const int sz{static_cast<int>(v.size())};
  for (int i=0; i!=sz; ++i)
  {
    if (v[i][i] != 0.0) return false;
  }
  return true;
}

bool sado::is_more_or_less_same(
    const std::vector<double> &v, const std::vector<double> &w)
{
  assert(v.size() == w.size());
  const int sz{static_cast<int>(v.size())};
  for (int i = 0; i != sz; ++i)
  {
    if (std::abs(v[i] - w[i]) > 0.0001)
      return false;
  }
  return true;
}

bool sado::is_regular_file(const std::string &filename) noexcept
{
  std::fstream f;
  f.open(filename.c_str(), std::ios::in);
  return f.is_open();
}

bool sado::is_square(const std::vector<std::vector<double>>& v)
{
  assert(!v.empty());
  return v.size() == v.back().size();
}

int sado::pick_random_individual_index(const int pop_size)
{
  return std::floor(Uniform() * pop_size);
}

std::vector<std::string>
sado::seperate_string(const std::string &input, const char seperator)
{
  std::vector<std::string> v;
  boost::algorithm::split(
      v,
      input,
      std::bind2nd(std::equal_to<char>(), seperator),
      boost::algorithm::token_compress_on);
  return v;
}

std::vector<double> sado::to_doubles(const std::vector<std::string> &v)
{
  std::vector<double> w;
  w.reserve(v.size());
  std::transform(
      std::begin(v),
      std::end(v),
      std::back_inserter(w),
      [](const std::string &s) { return std::stod(s); });
  return w;
}
