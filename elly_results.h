#ifndef ELLY_RESULTS_H
#define ELLY_RESULTS_H

#include <iosfwd>
#include <vector>
#include "elly_result.h"

namespace elly {

class results
{
public:

private:

  std::vector<result> m_results;

  friend std::ostream& operator<<(std::ostream& os, const results& r) noexcept;
};

std::ostream& operator<<(std::ostream& os, const results& r) noexcept;

} //~namespace elly


#endif // ELLY_RESULTS_H
