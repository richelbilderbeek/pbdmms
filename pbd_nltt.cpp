#include "pbd_nltt.h"

#include <cassert>
#include <iostream>
#include "pbd_helper.h"

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
