#ifndef KEWE_SIMULATION_H
#define KEWE_SIMULATION_H

#include <random>
#include <vector>
#include "kewe_results.h"
#include "kewe_individual.h"

class simulation
{

private:
  const kewe_parameters m_parameters;
  std::mt19937 m_generator;
  results m_results;
  result_variables m_output;
  std::vector<indiv> m_pop;
  int m_number_generations;
public:
  /// Default construction has testing parameters (FOR NOW)
  simulation(const kewe_parameters& parameters);
  void run();

  void set_pop(const std::vector<indiv>& pop) {m_pop = pop;}
  void set_generator(std::mt19937& generator) {m_generator = generator;}
  void set_results(const results& results) {m_results = results;}
  void set_output(const result_variables& output) {m_output = output;}

  void add_generation_number() {++m_number_generations;}

  std::mt19937 get_generator() {return m_generator;}
  result_variables get_output() const {return m_output;}
  results get_results() const {return m_results;}
  std::vector<indiv> get_pop() const{return m_pop;}
  kewe_parameters get_parameters() const { return m_parameters;}
  int get_generation_number() const {return m_number_generations;}

};

#endif // KEWE_SIMULATION_H
