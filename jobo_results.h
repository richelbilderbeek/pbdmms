#ifndef JOBO_RESULTS_H
#define JOBO_RESULTS_H

#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"
#include "jobo_output.h"
#include <stdexcept>

using namespace std;
namespace jobo {

class results
{
public:
  results(
  //The lineages through time
  std::vector<int> m_ltt
      );
};

std::ostream& operator<<(std::ostream& os, const results& r) noexcept;

} //~namespace jobo

#endif // JOBO_RESULTS_H
