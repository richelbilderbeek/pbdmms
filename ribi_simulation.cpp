#include "ribi_simulation.h"

#include <algorithm>
#include <fstream>
#include <random>
#include <boost/graph/isomorphism.hpp>
#include "ribi_individual.h"
#include "ribi_results.h"
//#include "ribi_helper.h"
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"

ribi::simulation::simulation(const parameters& p)
  : m_current_generation{0},
    m_parameters{p},
    m_population(
      p.get_population_size(),
      individual(
        individual::pin_t(std::string(p.get_n_pin_loci(),'A')),
        individual::sil_t(p.get_n_sil_loci(),0b00000)
      )
    ),
    m_results(p.get_max_genetic_distance()),
    m_rng_engine(p.get_rng_seed())
{

}

void ribi::simulation::do_one_timestep()
{
  const int population_size{m_parameters.get_population_size()};
  const int max_genetic_distance{m_parameters.get_max_genetic_distance()};
  const double pin_mutation_rate{m_parameters.get_pin_mutation_rate()};
  const double sil_mutation_rate{m_parameters.get_sil_mutation_rate()};
  const int sampling_interval{m_parameters.get_sampling_interval()};
  const size_t n_pin_loci{m_parameters.get_n_pin_loci()};
  const size_t n_sil_loci{m_parameters.get_n_sil_loci()};

  std::uniform_int_distribution<int> pin_index(0, n_pin_loci - 1);
  std::uniform_int_distribution<int> sil_index(0, n_sil_loci - 1);
  //mat_pin_inherit: inherits which PINs from mother?
  //Must be of same data type as boost::dynamic_bitset second constructor argument
  std::uniform_int_distribution<unsigned long> mat_pin_inherit(0,(1 << n_pin_loci) - 1);
  //mat_sil_inherit: inherits which PINs from mother?
  //Must be of same data type as boost::dynamic_bitset second constructor argument
  std::uniform_int_distribution<unsigned long> mat_sil_inherit(0,(1 << n_sil_loci) - 1);
  std::uniform_real_distribution<double> chance(0.0, 1.0);

  std::uniform_int_distribution<int> population_indices(0,population_size-1);

  //Find suitable father and mother
  int random_father_index{population_indices(m_rng_engine)};
  int random_mother_index{population_indices(m_rng_engine)};
  while (
    get_genetic_distance(
      m_population[random_mother_index],
      m_population[random_father_index]
    ) > max_genetic_distance
  )
  {
    random_father_index = population_indices(m_rng_engine);
    random_mother_index = population_indices(m_rng_engine);
  }

  //Only sample when something will happen
  if (m_current_generation % sampling_interval == 0)
  {
    m_results.add_measurement(
      m_current_generation, m_population
    );
  }

  const boost::dynamic_bitset<> pin_inheritance{
    n_pin_loci, mat_pin_inherit(m_rng_engine)
  };
  const boost::dynamic_bitset<> sil_inheritance{
    n_sil_loci, mat_sil_inherit(m_rng_engine)
  };
  const int random_kid_index{population_indices(m_rng_engine)};
  auto kid = create_offspring(
    m_population[random_mother_index],
    m_population[random_father_index],
    pin_inheritance,
    sil_inheritance
  );
  if (chance(m_rng_engine) < sil_mutation_rate) {
    kid.get_sil().flip(sil_index(m_rng_engine));
  }
  //Would freeze if no check for n_pin_loci > 0
  if (n_pin_loci && chance(m_rng_engine) < pin_mutation_rate)
  {
    kid.get_pin().change(pin_index(m_rng_engine), m_rng_engine);
  }
  m_population[random_kid_index] = kid;
}

void ribi::simulation::run()
{
  const int n_generations{m_parameters.get_n_generations()};

  //Overlapping generations, a timestep is defined by the creation of a new kid
  for (int t{0}; t!=n_generations; ++t)
  {
    do_one_timestep();
  }

  m_results.save_all(m_parameters.get_filename_genotype_frequency_graph());
}

void ribi::do_simulation(const parameters& p)
{
  simulation s(p);
  s.run();
}

void ribi::do_simulation_cpp(
  const int max_genetic_distance,
  const int n_generations,
  const int n_sil, //Use int over std::size_t for r
  const int n_pin, //Use int over std::size_t for r
  const double pin_mutation_rate,
  const int population_size,
  const std::string& rgfgraph_filename, //results_genotype_frequency_graph_filename
  const int rng_seed,
  const int sampling_interval,
  const double sil_mutation_rate
)
{
  const parameters p(
    max_genetic_distance,
    n_generations,
    n_pin,
    n_sil,
    pin_mutation_rate,
    population_size,
    rgfgraph_filename,
    rng_seed,
    sampling_interval,
    sil_mutation_rate
  );
  do_simulation(p);
}
