#ifndef DAIC_RESULTS_H
#define DAIC_RESULTS_H

#include <iosfwd>

namespace daic {

class results
{

};

std::ostream& operator<<(std::ostream& os, const results& r) noexcept;

} //~namespace daic

#endif // DAIC_RESULTS_H
