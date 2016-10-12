#include "ribi_parameters.h"
#include <sstream>
#include <stdexcept>
#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/predicate.hpp>

ribi::parameters::parameters(
  const int max_genetic_distance,
  const int n_generations,
  const std::size_t n_pin_loci,
  const std::size_t n_sil_loci,
  const double pin_mutation_rate,
  const int population_size,
  const std::string& rgfgraph_filename, //results_genotype_frequency_graph_filename
  const unsigned int rng_seed,
  const int sampling_interval,
  const double sil_mutation_rate
)
  : m_max_genetic_distance{max_genetic_distance},
    m_n_generations{n_generations},
    m_n_pin_loci{n_pin_loci},
    m_n_sil_loci{n_sil_loci},
    m_pin_mutation_rate{pin_mutation_rate},
    m_population_size{population_size},
    m_results_genotype_frequency_graph_filename{rgfgraph_filename},
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
  if (!boost::ends_with(rgfgraph_filename, ".dot"))
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "results_genotype_frequency_graph_filename must end with '.dot'"
      << ", filename given was '" << rgfgraph_filename << "'"
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

bool ribi::operator==(const parameters& lhs, const parameters& rhs) noexcept
{
  return
    lhs.get_filename_genotype_frequency_graph() == rhs.get_filename_genotype_frequency_graph()
    && lhs.get_max_genetic_distance() == rhs.get_max_genetic_distance()
    && lhs.get_n_generations() == rhs.get_n_generations()
    && lhs.get_n_pin_loci() == rhs.get_n_pin_loci()
    && lhs.get_n_sil_loci() == rhs.get_n_sil_loci()
    && lhs.get_pin_mutation_rate() == rhs.get_pin_mutation_rate()
    && lhs.get_population_size() == rhs.get_population_size()
    && lhs.get_rng_seed() == rhs.get_rng_seed()
    && lhs.get_sampling_interval() == rhs.get_sampling_interval()
    && lhs.get_sil_mutation_rate() == rhs.get_sil_mutation_rate()
  ;
}

bool ribi::operator!=(const parameters& lhs, const parameters& rhs) noexcept
{
  return !(lhs == rhs);
}

ribi::parameters ribi::create_test_parameters_1() noexcept
{
  const int max_genetic_distance{1};
  const int n_generations{3};
  const int n_pin_loci{2};
  const int n_sil_loci{2};
  const double pin_mutation_rate{0.1}; //Chance to have 1 locus flipped in a genome
  const int population_size{3};
  const std::string results_genotype_frequency_graph_filename{"test_parameters_1.dot"};
  const int rng_seed{42};
  const int sampling_interval{1};
  const double sil_mutation_rate{0.25}; //Chance to have 1 locus flipped in a genome
  return parameters(
    max_genetic_distance,
    n_generations,
    n_pin_loci,
    n_sil_loci,
    pin_mutation_rate,
    population_size,
    results_genotype_frequency_graph_filename,
    rng_seed,
    sampling_interval,
    sil_mutation_rate
  );
}

ribi::parameters ribi::create_test_parameters_2() noexcept
{
  const int max_genetic_distance{1};
  const int n_generations{1650};
  const int n_pin_loci{5};
  const int n_sil_loci{4};
  const double pin_mutation_rate{0.1}; //Chance to have 1 locus flipped in a genome
  const int population_size{8};
  const std::string results_genotype_frequency_graph_filename{"test_do_simulation_run_example_sim.dot"};
  const int rng_seed{30};
  const int sampling_interval{150};
  const double sil_mutation_rate{0.1}; //Chance to have 1 locus flipped in a genome
  return parameters(
    max_genetic_distance,
    n_generations,
    n_pin_loci,
    n_sil_loci,
    pin_mutation_rate,
    population_size,
    results_genotype_frequency_graph_filename,
    rng_seed,
    sampling_interval,
    sil_mutation_rate
  );
}

