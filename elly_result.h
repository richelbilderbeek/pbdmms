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

///The way a result is represented as a string
std::string result_to_str(
  const species_id id,
  const species_id pid,
  const double t_birth_mainland,
  const double t_birth_island,
  const double t_ext_mainland,
  const double t_ext_island,
  const double t_colonization,
  const clade_id cid
);

std::ostream& operator<<(std::ostream& os, const result& r) noexcept;

} //~namespace elly

#endif // ELLY_RESULT_H
