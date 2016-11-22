#ifndef JOBO_RESULTS_H
#define JOBO_RESULTS_H

#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"
#include "jobo_output.h"
#include <stdexcept>

using namespace std;
namespace jobo {

class jobo_results
{
public:
  //The lineages through time
  std::vector<int> m_ltt;
};

} //~namespace jobo

#endif // JOBO_RESULTS_H
