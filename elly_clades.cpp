#include "elly_clades.h"

#include <algorithm>
#include "elly_clade.h"

elly::clades elly::get_islanders(const clades& v)
{
  elly::clades w;
  std::transform(
    std::begin(v),
    std::end(v),
    std::back_inserter(w),
    [](const clade& c)
    {
      return get_islanders(c);
    }
  );
  return w;
}


