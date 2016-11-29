#ifndef JOBO_INDIVIDUALS_H
#define JOBO_INDIVIDUALS_H

#include <vector>
#include "jobo_individual.h"

namespace jobo {

using individuals = std::vector<individual>;

std::ostream& operator<<(std::ostream& os, const individuals& individuals) noexcept;

} //~namespace jobo

#endif // JOBO_INDIVIDUALS_H


