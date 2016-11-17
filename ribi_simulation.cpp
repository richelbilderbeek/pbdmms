#include "ribi_simulation.h"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <boost/graph/isomorphism.hpp>
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"
#include "ribi_individual.h"
#include "ribi_results.h"

ribi::simulation::simulation(const parameters& p)
  : m_current_generation{0},
    m_hopefull_monsters{},
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

void ribi::clean_simulation(const parameters& p)
{
  const auto user_filename = p.get_filename_genotype_frequency_graph();
  const std::vector<std::string> filenames = {
    get_filename_bs_dot(user_filename),
    get_filename_bs_svg(user_filename),
    get_filename_bs_png(user_filename),
    get_filename_dot(user_filename),
    get_filename_svg(user_filename),
    get_filename_png(user_filename)
  };
  for (const auto& filename: filenames)
  {
    std::remove(filename.c_str());
  }
}

ribi::individual ribi::simulation::create_kid(
  const std::pair<individual, individual>& parents
)
{
  const size_t n_pin_loci{m_parameters.get_n_pin_loci()};
  const size_t n_sil_loci{m_parameters.get_n_sil_loci()};

  //mat_pin_inherit: inherits which PINs from mother?
  //Must be of same data type as boost::dynamic_bitset second constructor argument
  std::uniform_int_distribution<unsigned long> mat_pin_inherit(0,(1 << n_pin_loci) - 1);
  //mat_sil_inherit: inherits which PINs from mother?
  //Must be of same data type as boost::dynamic_bitset second constructor argument
  std::uniform_int_distribution<unsigned long> mat_sil_inherit(0,(1 << n_sil_loci) - 1);

  const boost::dynamic_bitset<> pin_inheritance{
    n_pin_loci, mat_pin_inherit(m_rng_engine)
  };
  const boost::dynamic_bitset<> sil_inheritance{
    n_sil_loci, mat_sil_inherit(m_rng_engine)
  };
  auto kid = create_offspring(
    parents.first,
    parents.second,
    pin_inheritance,
    sil_inheritance
  );

  mutate(
    kid,
    m_parameters.get_pin_mutation_rate(),
    m_parameters.get_sil_mutation_rate(),
    m_rng_engine
  );
  return kid;
}

void ribi::simulation::do_one_timestep()
{
  const int population_size{m_parameters.get_population_size()};

  std::uniform_int_distribution<int> population_indices(0,population_size-1);

  //Find suitable father and mother. These can be the same
  const std::pair<individual, individual> parents = find_parents();

  //Replace an individuals by the parents' kid
  const individual kid = create_kid(parents);
  const int random_kid_index{population_indices(m_rng_engine)};
  m_population[random_kid_index] = kid;

  //Keep track of kids that cannot mate with parents
  if (kid_is_hopefull_monster(kid, parents, m_parameters.get_max_genetic_distance()))
  {
    m_hopefull_monsters.push_back(
      hopefull_monster(
        m_current_generation + 1, //kid is from the next generation
        kid,
        parents
      )
    );
  }

  m_results.add_measurement(
    m_current_generation,
    m_population,
    m_hopefull_monsters
  );

  //Go to the next generation
  ++m_current_generation;

}

std::pair<ribi::individual, ribi::individual> ribi::simulation::find_parents()
{
  return ::ribi::find_parents(
    m_population,
    m_parameters.get_max_genetic_distance(),
    m_rng_engine
  );
}

std::pair<ribi::individual, ribi::individual>
ribi::find_parents(
  const population& population,
  const int max_genetic_distance,
  std::mt19937& rng_engine
)
{
  const int population_size{static_cast<int>(population.size())};
  std::uniform_int_distribution<int> population_indices(0,population_size-1);
  int random_father_index{population_indices(rng_engine)};
  int random_mother_index{population_indices(rng_engine)};

  int n_tries{0};

  while (!can_mate(
    population[random_mother_index],
    population[random_father_index],
    max_genetic_distance
    )
  )
  {
    random_father_index = population_indices(rng_engine);
    random_mother_index = population_indices(rng_engine);
    ++n_tries;
    if (n_tries == 1000)
    {
      std::clog << "Warning: 1000 failed attempts\n";
    }
  }
  return std::make_pair(
    population[random_mother_index],
    population[random_father_index]
  );
}

bool ribi::kid_is_hopefull_monster(
  const individual& kid,
  const std::pair<individual, individual>& parents,
  const int max_genetic_distance
)
{
  return !can_mate(kid, parents.first, max_genetic_distance)
    && !can_mate(kid, parents.second, max_genetic_distance)
  ;
}


void ribi::simulation::run()
{
  const int n_generations{m_parameters.get_n_generations()};

  //Overlapping generations, a timestep is defined by the creation of a new kid
  for (int t{0}; t!=n_generations; ++t)
  {
    do_one_timestep();
  }

  m_results.save(m_parameters.get_filename_genotype_frequency_graph());
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
