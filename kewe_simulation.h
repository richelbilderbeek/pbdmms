#ifndef KEWE_SIMULATION_H
#define KEWE_SIMULATION_H

#include <random>
#include <vector>
#include "kewe_results.h"
#include "kewe_individual.h"

namespace kewe {

class simulation
{
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

  auto& get_generator() noexcept { return m_generator;}
  const auto& get_result_variables() const noexcept { return m_output;}
  const auto& get_results() const noexcept { return m_results;}
  const auto& get_pop() const noexcept { return m_pop;}
  const auto& get_parameters() const noexcept { return m_parameters;}
  int get_generation_number() const noexcept { return m_number_generations;}
  const auto& get_ltt_plot() const noexcept { return m_ltt_plot;}

private:
  const parameters m_parameters;
  std::mt19937 m_generator;
  results m_results;
  result_variables m_output;
  individuals m_pop;
  int m_number_generations;
  std::vector<std::pair<int,int>> m_ltt_plot;

  void do_measurements(const int t);

  void reserve_space_output_vectors(
    result_variables& output_variables,
    std::vector<std::vector<double>>& histX,
    std::vector<std::vector<double>>& histP,
    std::vector<std::vector<double>>& histQ,
    const parameters& p
  );
};


bool has_bimodal_eco_types(const simulation& s);

bool has_branching_mating(const simulation& s);

bool has_sympatric_speciation(const simulation& s);

bool must_do_measurements(const int t, const simulation& s);

} //~namespace kewe

#endif // KEWE_SIMULATION_H
