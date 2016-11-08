#ifndef KEWE_SIMULATION_H
#define KEWE_SIMULATION_H

#include "kewe_results.h"

class simulation
{

private:
  results m_results;
public:
  /// Default construction has testing parameters (FOR NOW)
  simulation();
  void run();
  results get_results() const {return m_results;}
};

#endif // KEWE_SIMULATION_H
