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

  ///Run one generation
  void do_timestep();

  void run();

  ///Overwrite the current generation be a next one. increases the time.
  ///If needed, do all measurements on current/old generation
  void set_pop(const individuals& pop);

  auto& get_generator() noexcept { return m_generator;}
  const auto& get_result_variables() const noexcept { return m_output;}
  const auto& get_results() const noexcept { return m_results;}
  const auto& get_pop() const noexcept { return m_pop;}
  const auto& get_parameters() const noexcept { return m_parameters;}
  int get_generation_number() const noexcept { return m_t;}
  const auto& get_ltt_plot() const noexcept { return m_ltt_plot;}

private:
  const parameters m_parameters;
  std::mt19937 m_generator;
  results m_results;
  result_variables m_output;
  individuals m_pop;

  ///Time, generation number
  int m_t;

  std::vector<std::pair<int,int>> m_ltt_plot;

  ///Measure the population now
  void do_measurements();

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

bool must_do_measurements(const simulation& s);

} //~namespace kewe

#endif // KEWE_SIMULATION_H
