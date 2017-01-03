#ifndef ELLY_RESULTS_H
#define ELLY_RESULTS_H

#include <iosfwd>

namespace elly {

class results
{

};

std::string get_results_header() noexcept;

std::ostream& operator<<(std::ostream& os, const results& r) noexcept;

} //~namespace elly


#endif // ELLY_RESULTS_H
