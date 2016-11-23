#include "pbd_nltt.h"

#include <cassert>
#include <iostream>
#include "pbd_helper.h"

pbd::nltt pbd::create_test_nltt() noexcept
{
  nltt p;
  p.push_back(std::make_pair(0.0, 0.1));
  p.push_back(std::make_pair(0.0, 0.3));
  p.push_back(std::make_pair(0.0, 0.4));
  p.push_back(std::make_pair(0.0, 0.5));
  p.push_back(std::make_pair(1.0, 1.0));
  return p;
}

pbd::nltt pbd::load_nltt_from_csv(const std::string& csv_filename)
{
  const std::vector<std::string> text{
    remove_first(
      file_to_vector(csv_filename)
    )
  };

  nltt my_nltt;
  if (text.size() <= 1) return my_nltt;
  my_nltt.reserve(text.size());

  for (const std::string& line: text)
  {
    const auto v = remove_first(seperate_string(line, ','));

    assert(v.size() == 2);
    const double t{std::stod(v[0])};
    const double n{std::stod(v[1])};
    my_nltt.push_back(std::make_pair(t, n));
  }
  return my_nltt;
}

std::ostream& pbd::operator<<(std::ostream& os, const nltt& l) noexcept
{
  os << "normalized_time,normalized_number_of_lineages\n";
  for (const auto p: l)
  {
    os << p.first << ',' << p.second << '\n';
  }
  return os;
}
