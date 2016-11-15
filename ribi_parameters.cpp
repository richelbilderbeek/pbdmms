#include "ribi_parameters.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include "is_regular_file.h"

///Checks if 't' is at least zero, throws otherwise
template <class T>
void must_be_at_least_zero(const std::string& name, const T& t)
{
  if (t < 0.0)
  {
    std::stringstream s;
    s << name << " must at least be zero";
    throw std::invalid_argument(s.str());
  }
  //OK
}

///Checks if 't' is at least one, throws otherwise
template <class T>
void must_be_at_least_one(const std::string& name, const T& t)
{
  if (t < 1)
  {
    std::stringstream s;
    s << name << " must at least be one";
    throw std::invalid_argument(s.str());
  }
  //OK
}

///Checks if 't' is at most one, throws otherwise
template <class T>
void must_be_at_most_one(const std::string& name, const T& t)
{
  if (t > 1.0)
  {
    std::stringstream s;
    s << name << " must at most be one";
    throw std::invalid_argument(s.str());
  }
  //OK
}

///Checks if 's' end with '.dot', throws otherwise
void must_end_with_dot(const std::string& name, const std::string& s)
{
  if (!boost::ends_with(s, ".dot"))
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << name << " must end with '.dot'"
      << ", value given was '" << s << "'"
    ;
    throw std::invalid_argument(msg.str());
  }
  //OK
}

///Checks if 's' has no spaces
void must_have_no_spaces(const std::string& name, const std::string& s)
{
  if (std::count(std::begin(s), std::end(s), ' '))
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << name << " must have no spaces"
      << ", value given was '" << s << "'"
    ;
    throw std::invalid_argument(msg.str());
  }
  //OK
}



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
  must_be_at_least_one("max_genetic_distance", m_max_genetic_distance);
  must_be_at_least_zero("n_generations", m_n_generations);
  must_be_at_least_zero("pin_mutation_rate", m_pin_mutation_rate);
  must_be_at_most_one("pin_mutation_rate", m_pin_mutation_rate);
  must_be_at_least_zero("population_size", m_population_size);
  must_end_with_dot("rgfgraph_filename", m_results_genotype_frequency_graph_filename);
  must_have_no_spaces("rgfgraph_filename", m_results_genotype_frequency_graph_filename);

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

  must_be_at_least_zero("sil_mutation_rate", m_sil_mutation_rate);
  must_be_at_most_one("sil_mutation_rate", m_sil_mutation_rate);
}

ribi::parameters ribi::create_profiling_parameters() noexcept
{
  const int max_genetic_distance{1};
  const int n_generations{100};
  const int n_pin_loci{4};
  const int n_sil_loci{4};
  const double pin_mutation_rate{0.5};
  const int population_size{10};
  const std::string rgfgraph_filename{"create_profiling_parameters.dot"};
  const int rng_seed{42};
  const int sampling_interval{1};
  const double sil_mutation_rate{0.5};
  return parameters(
    max_genetic_distance,
    n_generations,
    n_pin_loci,
    n_sil_loci,
    pin_mutation_rate,
    population_size,
    rgfgraph_filename,
    rng_seed,
    sampling_interval,
    sil_mutation_rate
  );
}

ribi::parameters ribi::create_test_parameters_1() noexcept
{
  const int max_genetic_distance{1};
  const int n_generations{3};
  const int n_pin_loci{2};
  const int n_sil_loci{2};
  const double pin_mutation_rate{0.1};
  const int population_size{3};
  //results_genotype_frequency_graph_filename
  const std::string rgfgraph_filename{"test_parameters_1.dot"};
  const int rng_seed{42};
  const int sampling_interval{1};
  const double sil_mutation_rate{0.25};
  return parameters(
    max_genetic_distance,
    n_generations,
    n_pin_loci,
    n_sil_loci,
    pin_mutation_rate,
    population_size,
    rgfgraph_filename,
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
  const double pin_mutation_rate{0.1};
  const int population_size{8};
  const std::string rgfgraph_filename{"create_test_parameters_2.dot"};
  const int rng_seed{30};
  const int sampling_interval{1};
  const double sil_mutation_rate{0.1};
  return parameters(
    max_genetic_distance,
    n_generations,
    n_pin_loci,
    n_sil_loci,
    pin_mutation_rate,
    population_size,
    rgfgraph_filename,
    rng_seed,
    sampling_interval,
    sil_mutation_rate
  );
}

ribi::parameters ribi::load_parameters(const std::string& filename)
{
  if (!is_regular_file(filename))
  {
    throw std::invalid_argument("parameter file cannot be found");
  }
  std::ifstream f(filename);
  parameters p = create_test_parameters_1();
  f >> p;
  return p;
}

void ribi::save_parameters(
  const parameters& p,
  const std::string& filename
)
{
  std::ofstream f(filename);
  f << p;
}

std::ostream& ribi::operator<<(std::ostream& os, const parameters& p)
{
  os
    << p.m_max_genetic_distance << " "
    << p.m_n_generations << " "
    << p.m_n_pin_loci << " "
    << p.m_n_sil_loci << " "
    << p.m_pin_mutation_rate << " "
    << p.m_population_size << " "
    << p.m_results_genotype_frequency_graph_filename << " "
    << p.m_rng_seed << " "
    << p.m_sampling_interval << " "
    << p.m_sil_mutation_rate
  ;
  return os;
}

std::istream& ribi::operator>>(std::istream& is, parameters& p)
{
  is
    >> p.m_max_genetic_distance
    >> p.m_n_generations
    >> p.m_n_pin_loci
    >> p.m_n_sil_loci
    >> p.m_pin_mutation_rate
    >> p.m_population_size
    >> p.m_results_genotype_frequency_graph_filename
    >> p.m_rng_seed
    >> p.m_sampling_interval
    >> p.m_sil_mutation_rate
  ;
  return is;
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
