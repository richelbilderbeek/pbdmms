#include "pbd_helper.h"

#include <boost/algorithm/string/find_iterator.hpp> //Line 248

#include <cassert>
#include <fstream>
#include <sstream>

#include <boost/algorithm/string/split.hpp>

void pbd::delete_file(const std::string& filename)
{
  if(!is_regular_file(filename))
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "can only delete existing files, "
      << "filename supplied: '"
      << filename << "' was not found"
    ;
    throw std::invalid_argument(msg.str());
  }
  std::remove(filename.c_str());

  if(is_regular_file(filename))
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "failed to delete existing file '"
      << filename << "'"
    ;
    throw std::invalid_argument(msg.str());
  }
}

std::vector<std::string> pbd::file_to_vector(const std::string& filename)
{
  if(!is_regular_file(filename))
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "can only convert existing files, "
      << "filename supplied: '"
      << filename << "' was not found"
    ;
    throw std::invalid_argument(msg.str());
  }
  assert(is_regular_file(filename));
  std::vector<std::string> v;
  std::ifstream in{filename.c_str()};
  assert(in.is_open());
  //Without this test in release mode,
  //the program might run indefinitely when the file does not exists
  for (int i=0; !in.eof(); ++i)
  {
    std::string s;
    std::getline(in,s);
    v.push_back(s); //Might throw std::bad_alloc
  }
  //Remove empty line at back of vector
  if (!v.empty() && v.back().empty()) v.pop_back();
  return v;
}


bool pbd::is_regular_file(const std::string& filename) noexcept
{
  std::fstream f;
  f.open(filename.c_str(),std::ios::in);
  return f.is_open();
}

std::vector<std::string> pbd::remove_first(
  std::vector<std::string> v
)
{
  if (v.empty())
  {
    throw std::invalid_argument("Cannot remove absent first line");
  }
  v.erase(std::begin(v));
  return v;
}


std::vector<std::string> pbd::seperate_string(
  const std::string& input,
  const char seperator
)
{
  std::istringstream is(input);
  std::vector<std::string> v;
  for (
    std::string sub;
    std::getline(is, sub, seperator);
    v.push_back(sub))
  {} //!OCLINT Indeed, this is an empty loop, and should be
  return v;
}
