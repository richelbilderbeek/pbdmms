#include "distancer_parameters.h"
#include <sstream>
#include <stdexcept>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/predicate.hpp>

parameters::parameters(
  const int max_genetic_distance,
  const int n_generations,
  const std::size_t n_pin_loci,
  const std::size_t n_sil_loci,
  const double pin_mutation_rate,
  const int population_size,
  const std::string& results_genotype_frequency_graph_filename,
  const int rng_seed,
  const int sampling_interval,
  const double sil_mutation_rate
)
  : m_max_genetic_distance{max_genetic_distance},
    m_n_generations{n_generations},
    m_n_pin_loci{n_pin_loci},
    m_n_sil_loci{n_sil_loci},
    m_pin_mutation_rate{pin_mutation_rate},
    m_population_size{population_size},
    m_results_genotype_frequency_graph_filename{results_genotype_frequency_graph_filename},
    m_rng_seed{rng_seed},
    m_sampling_interval{sampling_interval},
    m_sil_mutation_rate{sil_mutation_rate}
{
  if (m_max_genetic_distance < 1)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "max_genetic_distance must be >= 1, "
      << "supplied value was " << m_max_genetic_distance
    ;
    throw std::invalid_argument(msg.str());
  }
  if (m_n_generations < 0)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "n_generations must be >= 0, "
      << "supplied value was " << m_n_generations
    ;
    throw std::invalid_argument(msg.str());
  }
  if (m_pin_mutation_rate < 0.0)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "PIN mutation rate must be >= 0.0, "
      << "supplied value was " << m_pin_mutation_rate
    ;
    throw std::invalid_argument(msg.str());
  }
  if (m_pin_mutation_rate > 1.0)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "PIN mutation rate must be <= 1.0, "
      << "supplied value was " << m_pin_mutation_rate
    ;
    throw std::invalid_argument(msg.str());
  }

  if (m_population_size < 0)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "population_size must be >= 0, "
      << "supplied value was " << m_population_size
    ;
    throw std::invalid_argument(msg.str());
  }
  if (!boost::ends_with(results_genotype_frequency_graph_filename, ".dot"))
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "results_genotype_frequency_graph_filename must end with '.dot'"
      << ", filename given was '" << results_genotype_frequency_graph_filename << "'"
    ;
    throw std::invalid_argument(msg.str());
  }

  //Allow a population of zero generations
  if (m_n_generations > 0 && m_sampling_interval < 1)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "sampling_interval must be >= 1, "
      << "supplied value was " << m_sampling_interval
    ;
    throw std::invalid_argument(msg.str());
  }
  //Allow a population of zero generations
  if (m_n_generations > 0 && m_n_generations < m_sampling_interval)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "sampling_interval must be >= n_generations "
      << "(otherwise only a measurement would be taken at the start), "
      << "sampling_interval: " << m_sampling_interval
      << "n_generations: " << m_n_generations
    ;
    throw std::invalid_argument(msg.str());
  }

  if (m_sil_mutation_rate < 0.0)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "SIL mutation rate must be >= 0.0, "
      << "supplied value was " << m_sil_mutation_rate
    ;
    throw std::invalid_argument(msg.str());
  }
  if (m_sil_mutation_rate > 1.0)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "SIL mutation rate must be <= 1.0, "
      << "supplied value was " << m_sil_mutation_rate
    ;
    throw std::invalid_argument(msg.str());
  }
}

/*
std::string parameters::get_filename_genotype_frequency_graph_before_summary() const noexcept
{
  std::string f = get_filename_genotype_frequency_graph();
  boost::replace_last(f, ".dot", "_before_summary.dot");
  return f;
}

std::string parameters::get_filename_genotype_frequency_graph_before_summary_as_png() const noexcept
{
  std::string f = get_filename_genotype_frequency_graph_before_summary();
  boost::replace_last(f, ".dot", ".png");
  return f;
}

std::string parameters::get_filename_genotype_frequency_graph_before_summary_as_svg() const noexcept
{
  std::string f = get_filename_genotype_frequency_graph_before_summary();
  boost::replace_last(f, ".dot", ".svg");
  return f;
}
*/
