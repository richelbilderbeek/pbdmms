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
#include "ribi_parameters.h"

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

ribi::population ribi::create_next_population(
  const simulation& s,
  std::mt19937& rng_engine
)
{
  const int population_size{static_cast<int>(s.get_population().size())};

  population next_population;
  next_population.reserve(population_size);
  for (int i=0; i!=population_size; ++i)
  {
    //Find suitable father and mother. These can be the same
    const std::pair<individual, individual> parents = find_parents(
      s.get_population(),
      s.get_parameters().get_max_genetic_distance(),
      rng_engine
    );

    //Replace an individuals by the parents' kid
    const individual kid = create_kid(parents, s, rng_engine);

    next_population.push_back(kid);
  }
  return next_population;
}

ribi::individual ribi::create_kid(
  const std::pair<individual, individual>& parents,
  const simulation& s,
  std::mt19937& rng_engine
)
{
  const size_t n_pin_loci{s.get_parameters().get_n_pin_loci()};
  const size_t n_sil_loci{s.get_parameters().get_n_sil_loci()};

  //mat_pin_inherit: inherits which PINs from mother?
  //Must be of same data type as boost::dynamic_bitset second constructor argument
  std::uniform_int_distribution<unsigned long> mat_pin_inherit(0,(1 << n_pin_loci) - 1);
  //mat_sil_inherit: inherits which PINs from mother?
  //Must be of same data type as boost::dynamic_bitset second constructor argument
  std::uniform_int_distribution<unsigned long> mat_sil_inherit(0,(1 << n_sil_loci) - 1);

  const boost::dynamic_bitset<> pin_inheritance{
    n_pin_loci, mat_pin_inherit(rng_engine)
  };
  const boost::dynamic_bitset<> sil_inheritance{
    n_sil_loci, mat_sil_inherit(rng_engine)
  };
  auto kid = create_offspring(
    parents.first,
    parents.second,
    pin_inheritance,
    sil_inheritance
  );

  mutate(
    kid,
    s.get_parameters().get_pin_mutation_rate(),
    s.get_parameters().get_sil_mutation_rate(),
    rng_engine
  );
  return kid;
}

ribi::simulation ribi::create_simulation(const parameters& p) noexcept
{
  return simulation(p);
}

void ribi::simulation::do_one_timestep()
{
  const int population_size{m_parameters.get_population_size()};

  std::uniform_int_distribution<int> population_indices(0,population_size-1);

  //Find suitable father and mother. These can be the same
  const std::pair<individual, individual> parents = find_parents(
    m_population,
    m_parameters.get_max_genetic_distance(),
    m_rng_engine
  );

  //Replace an individuals by the parents' kid
  const individual kid = create_kid(parents, *this, m_rng_engine);
  const int random_kid_index{population_indices(m_rng_engine)};
  m_population[random_kid_index] = kid;

  #ifdef FIX_ISSUE_41
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
  #endif // FIX_ISSUE_41

  m_results.add_measurement(
    m_current_generation,
    m_population,
    m_hopefull_monsters
  );

  //Go to the next generation
  ++m_current_generation;

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

ribi::results ribi::get_results(const simulation& s) noexcept
{
  return s.get_results();
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

void ribi::simulation::set_population(population p)
{
  m_results.add_measurement(m_current_generation, m_population);
  m_population = p;
  ++m_current_generation;
}

void ribi::set_population(simulation& s, population p)
{
  s.set_population(p);
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
  const probability pin_mutation_rate,
  const int population_size,
  const std::string& rgfgraph_filename, //results_genotype_frequency_graph_filename
  const int rng_seed,
  const probability sil_mutation_rate
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
    sil_mutation_rate
  );
  do_simulation(p);
}
