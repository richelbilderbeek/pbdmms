#ifndef ELLY_RESULT_H
#define ELLY_RESULT_H

#include <iosfwd>
#include "elly_species.h"
#include "elly_species_id.h"

namespace elly {

///Extract the history of a species
class result
{
public:
  result(const species& s);

private:
  const species m_s;

  friend std::ostream& operator<<(std::ostream& os, const result& r) noexcept;
};

std::string get_result_header() noexcept;

std::ostream& operator<<(std::ostream& os, const result& r) noexcept;

} //~namespace elly

#endif // ELLY_RESULT_H
