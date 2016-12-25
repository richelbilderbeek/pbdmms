#ifndef JOBO_INDIVIDUALS_H
#define JOBO_INDIVIDUALS_H

#include <vector>
#include "jobo_individual.h"

namespace jobo {

using individuals = std::vector<individual>;

std::vector<double> calc_fitnesses(const individuals& population) noexcept;

///Must have at last two individuals with fitness > 0.0
bool is_viable(const individuals& population);

std::ostream& operator<<(std::ostream& os, const individuals& individuals) noexcept;

} //~namespace jobo

#endif // JOBO_INDIVIDUALS_H


