#include "elly_clades.h"

#include <algorithm>
#include "elly_clade.h"

int elly::count_empty(const clades& cs) noexcept
{
  return std::count_if(
    std::begin(cs),
    std::end(cs),
    [](const clade& c)
    {
      return c.get_species().empty();
    }
  );
}

elly::clades elly::get_islanders(const clades& v)
{
  clades w;
  std::transform(
    std::begin(v),
    std::end(v),
    std::back_inserter(w),
    [](const clade& c)
    {
      return clade(get_islanders(c.get_species()));
    }
  );
  return w;
}

elly::clades elly::overestimate_colonization_times(const clades& v)
{
  clades w;
  std::transform(
    std::begin(v),
    std::end(v),
    std::back_inserter(w),
    [](const clade& c)
    {
      try
      {
        return overestimate_colonization_time(c);
      }
      catch (std::logic_error&)
      {
        return c;
      }
    }
  );
  return w;
}

elly::clades elly::get_non_empty_clades(const clades& v) noexcept
{
  clades w;
  std::copy_if(
    std::begin(v),
    std::end(v),
    std::back_inserter(w),
    [](const clade& c)
    {
      return !c.get_species().empty();
    }
  );
  return w;
}
