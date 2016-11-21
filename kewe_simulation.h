#ifndef KEWE_SIMULATION_H
#define KEWE_SIMULATION_H

#include <random>
#include "kewe_results.h"

class simulation
{

private:
  const kewe_parameters m_parameters;
  std::default_random_engine m_generator;
  results m_results;
  result_variables m_output;
public:
  /// Default construction has testing parameters (FOR NOW)
  simulation(const kewe_parameters& parameters);
  void run();
  std::default_random_engine get_generator() {return m_generator;}
  result_variables get_output() const {return m_output;}
  results get_results() const {return m_results;}
  kewe_parameters get_parameters() const { return m_parameters;}

};

#endif // KEWE_SIMULATION_H
