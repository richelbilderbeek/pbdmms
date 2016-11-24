#include "jobo_simulation.h"
#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"
#include "jobo_results.h"

using namespace std;
using namespace jobo;

jobo::results::results(
  const simulation& simulation
) noexcept
: m_ltt{ltt}
{
}

std::ostream& operator<<(std::ostream& os, const results& r) noexcept;
