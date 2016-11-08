#ifndef RIBI_CREATE_TALLY_H
#define RIBI_CREATE_TALLY_H

#include <vector>

namespace ribi {

///Create a tally of value occurrances
///For example {0,1,1,2,2,2} would result in {1,2,3}
///The sum of the tally will be equal to the length of the input vector
std::vector<int> create_tally(const std::vector<int>& v) noexcept;

} //~namespace ribi

#endif // RIBI_CREATE_TALLY_H
