#ifndef KEWE_SIMULATION_H
#define KEWE_SIMULATION_H

#include <random>
#include <vector>
#include "kewe_results.h"
#include "kewe_individual.h"

namespace kewe {

class simulation
{

private:
  const parameters m_parameters;
  std::mt19937 m_generator;
  results m_results;
  result_variables m_output;
  individuals m_pop;
  int m_number_generations;
  std::vector<std::pair<int,int>> m_ltt_plot;
public:
  /// Default construction has testing parameters (FOR NOW)
  simulation(const parameters& parameters);
  void run();

  void set_pop(const individuals& pop) {m_pop = pop;}
  void set_generator(std::mt19937& generator) {m_generator = generator;}
  void set_results(const results& results) {m_results = results;}
  void set_result_variables(const result_variables& output) {m_output = output;}
  void set_ltt_plot(const std::vector<std::pair<int, int>>& ltt_plot){m_ltt_plot = ltt_plot;}

  void add_generation_number() {++m_number_generations;}

  std::mt19937& get_generator() {return m_generator;}
  result_variables get_result_variables() const {return m_output;}
  results get_results() const {return m_results;}
  individuals get_pop() const{return m_pop;}
  parameters get_parameters() const { return m_parameters;}
  int get_generation_number() const {return m_number_generations;}
  std::vector<std::pair<int,int>> get_ltt_plot() const {return m_ltt_plot;}

  void reserve_space_output_vectors(
      result_variables& output_variables,
      std::vector<std::vector<double>>& histX,
      std::vector<std::vector<double>>& histP,
      std::vector<std::vector<double>>& histQ,
      const parameters& p);

};

bool has_bimodal_eco_types(const simulation& s);

bool has_branching_mating(const simulation& s);

bool has_sympatric_speciation(const simulation& s);

} //~namespace kewe

#endif // KEWE_SIMULATION_H
