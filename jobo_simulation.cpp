#include "jobo_simulation.h"
#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"
#include "jobo_results.h"
#include "jobo_jkr_adapters.h"
#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <cstdio>
#include <cctype>
#include <string>
#include <stdexcept>
#include <random>
#include <cmath>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include "count_undirected_graph_connected_components.h"
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"
#include "count_max_number_of_pieces.h"


using namespace std;
using namespace jobo;

jobo::simulation::simulation(
  const parameters& parameters
) noexcept
  : m_individuals{create_initial_population(parameters)},
    m_parameters{parameters},
    m_rng_engine(parameters.get_seed()),
    m_results{}
{
}

individuals jobo::create_initial_population(const parameters& parameters)
{
  const std::vector<individual> population(
    parameters.get_population_size(),
    individual(create_initial_genotype(parameters.get_n_loci()))
  );
  assert(static_cast<int>(population.size()) == parameters.get_population_size());
  assert(population.back().get_n_loci() == parameters.get_n_loci());
  return population;
}

void jobo::simulation::do_timestep()
{
  const auto next_population
    = create_next_population(*this, m_rng_engine);
  set_population(*this, next_population);
}

void jobo::simulation::run()
{
  const int n_generations = get_n_generations(this->get_parameters());
  for (int t=0; t!=n_generations; ++t)
  {
    do_timestep();
  }
  //Measure at the present
  //set_population(m_individuals);


   //todo fix functions, *this
  this->get_results().add_nltt_viables(this->get_results().get_ltt_viables());
  this->get_results().add_nltt_inviables(this->get_results().get_ltt_inviables());
  //save_ltt_plot(get_results(), get_ltt_plot_filename(this->get_parameters()));
  save_ltt_plot_viables(get_results(),get_ltt_plot_viables_filename(this->get_parameters()));
  save_ltt_plot_inviables(get_results(),get_ltt_plot_inviables_filename(this->get_parameters()));
  save_nltt_plot_viables(get_results(),get_nltt_plot_viables_filename(this->get_parameters()));
  save_nltt_plot_inviables(get_results(),get_nltt_plot_inviables_filename(this->get_parameters()));
}

void jobo::simulation::set_individuals(const individuals& is)
{
  this->m_individuals = is;
}

std::vector<int> jobo::get_random_ints(std::mt19937& rng_engine, const int& n)
{
  // Use number of loci to get number of random ints with 1 seed
  if (n < 0)
  {
    throw std::invalid_argument("number of ints must be positive");
  }
  std::vector<int> n_loci_ints;
  n_loci_ints.resize(n);
  std::uniform_int_distribution<int> distribution(0,100);
  for (int i=0; i!=n; ++i)
  {
    int w = distribution(rng_engine);
    assert (w >=0);
    assert (w <=100);
    n_loci_ints[i] =  w;
  }
  // Return all random ints in one vector
  return n_loci_ints;
}

std::vector<double> jobo::get_random_doubles(std::mt19937& rng_engine, const int& n)
{
  // Use number of loci to get number of random doubles with 1 seed
  if (n < 0)
  {
    throw std::invalid_argument("number of doubles must be positive");
  }
  std::vector<double> n_loci_doubles;
  n_loci_doubles.resize(n);
  std::uniform_real_distribution<double> distribution(0,1);
  for (int i=0; i!=n; ++i)
  {
    double w = distribution(rng_engine);
    assert (w >=0);
    assert (w <=1);
    n_loci_doubles[i] =  w;
  }
  // Return all random doubles in one vector
  return n_loci_doubles;
}

int jobo::get_random_parent(
    std::mt19937& rng_engine,
    const int& population_size
)
{
  std::uniform_int_distribution<int> distribution(0,population_size-1);
  int random_parent = distribution(rng_engine);
  assert(random_parent >= 0);
  assert(random_parent <= population_size);
  return random_parent;
}

jobo::individuals jobo::
create_next_generation(
    const individuals& population_raw,
    const parameters& ps,
    std::mt19937& rng_engine
)
{
  if (!is_viable(population_raw))
  {
    throw std::runtime_error("Population has become inviable");
  }
  //Only put fit individuals in 'population'
  individuals population = remove_inviable_species(population_raw);

  const double mutation_rate{ps.get_mutation_rate()};
  const int population_size{ps.get_population_size()};
  const auto fitnesses = calc_fitnesses(population);
  std::discrete_distribution<> d(std::begin(fitnesses), std::end(fitnesses));
  assert(ps.get_population_size() >= static_cast<int>(population.size()));

  individuals new_population;
  new_population.reserve(population_size);

  // 2. Get loop to repeat create_offspring by the number of constant population size
  for (int i=0; i!=population_size; ++i)
  {
    // 3. Get random father, pick random individual from vector
    int number_father = d(rng_engine);
    int number_mother = d(rng_engine);

    while (number_father == number_mother)
    {
      number_mother = d(rng_engine);
    }
    /*
    int number_father = get_random_parent(rng_engine,population_size);
    int number_mother;
    do {number_mother = get_random_parent(rng_engine,population_size);}
    while (number_father == number_mother);
    // Parents can't be one and the same!
    */
    assert(number_father != number_mother);
    const individual father = population[number_father];
    const individual mother = population[number_mother];
    const individual clean_offspring = create_offspring(mother, father, rng_engine);
    const individual offspring = create_mutation(
      clean_offspring,
      mutation_rate,
      rng_engine
    );
    new_population.push_back(offspring);
  }
  return new_population;
}

/*
std::vector<individual> jobo::connect_generations(
    std::vector<individual> individuals,
    const double mutation_rate,
    std::mt19937& rng_engine
)
{
  //Make circle complete with goto_next_generation
  std::vector<individual> new_individuals = goto_next_generation(
    individuals,mutation_rate,rng_engine);
  std::vector<individual> living_individuals = extinction_low_fitness(new_individuals);

  //Translate living_individuals into individuals
  individuals = living_individuals;
  new_individuals = living_individuals;
return individuals;
}
*/

std::vector<genotype> jobo::get_unique_genotypes(
    const std::vector<individual>& individuals
)
{
  const int population_size{static_cast<int>(individuals.size())};
  // Run through population to collect all genotypes
  set<std::string> set_of_genotypes;
  for (int i=0; i!=population_size; ++i)
  {
    // Create set function to store all unique genotypes, always one or more
    const individual w = individuals[i];
    set_of_genotypes.insert(w.get_genotype());
  }
  // Change set_of_genotypes into vector_of_genotypes
  vector<string>vector_of_genotypes(
    set_of_genotypes.begin(),
    set_of_genotypes.end()
  );
  // Return set with all unique genotypes
  return vector_of_genotypes;
}

double jobo::calc_chance_dead_kids(
    const genotype& w,
    const genotype& q
)
{
  // Test if both genotypes have same size
  assert(w.size() == q.size());
  const int wz{static_cast<int>(w.size())};
  vector<double> chs_dead_offspring;
  for (int i=0; i!=wz; i+=2)
  {
    double ch_dead_offspring{0.5};
    // Test if both first loci are upper case letters
    if(w[i] == q[i] && (std::isupper(w[i])))
    {
    ch_dead_offspring = (ch_dead_offspring-0.5);
    }
    // Test if both second loci are lower case letters
    if(w[i+1] == q[i+1] && (std::islower(w[i+1])))
    {
    ch_dead_offspring = (ch_dead_offspring-0.5);
    }
    // Calculate the chance for dead offpsring for the rest group
    if(ch_dead_offspring == 0.5)
    {
    ch_dead_offspring = (ch_dead_offspring+0.5);
    }
    chs_dead_offspring.push_back(ch_dead_offspring);
  }
  // Calculate the chance of dead offspring for all loci couples together
  double chance_dead_kids = 0;
  std::for_each(chs_dead_offspring.begin(), chs_dead_offspring.end(),
  [&] (double n) {
    chance_dead_kids += n;
  });
  chance_dead_kids = chance_dead_kids/(w.size()/2);
  return chance_dead_kids;
}

vector<genotype> jobo::collect_viable_genotypes(const std::vector<individual>& individuals)
{
  // Ditch the duplicates to speed up the calculation
  const std::vector<genotype> z = get_unique_genotypes(individuals);
  assert(z.size()>0);
  const int sz{static_cast<int>(z.size())};
  std::vector<genotype> viable_genotypes;
  for (int i=0; i!=sz; i+=1)
  {
    const genotype w = z[i];
    if (is_viable_species(w))
    {
      viable_genotypes.push_back(w);
    }
  }
  //cout << viable_population.size() << ".\n";
  assert(viable_genotypes.size()>0);
  assert(is_viable_species(viable_genotypes.back()));
  return viable_genotypes;
}

/*
std::vector<double> chances_dead_kids_for_each_genotype;
for(int i=0; i!=gs; ++i)
{
  for(int j=0; i!=gs; ++i)
  {
    double chance_dead_kids_for_each_genotype = calc_chance_dead_kids(vector_of_genotypes[i],vector_of_genotypes[j]);
    chances_dead_kids_for_each_genotype.push_back(chance_dead_kids_for_each_genotype);
  }
}

return chances_dead_kids;
}
*/

/*
int jobo::get_n_good_species(
    std::vector<double> chances_dead_kids,
    std::set<genotype> set_of_genotypes
)
{
std::vector<std::string> vector_of_genotypes(set_of_genotypes.begin(), set_of_genotypes.end());

const int gs{static_cast<int>(vector_of_genotypes.size())};
const int gc{static_cast<int>(chances_dead_kids.size())};
*/
/*
std::vector<std::string> group_1(vector_of_genotypes[1]);
for(int i=0; i!=gs-1; ++i)
{
  if (chances_dead_kids_for_each_genotype[i]==0)
  {
  group_1.push_back(vector_of_genotypes[i]);
  }
}

for(int i=0; i!=gs; ++i)
if(std::find(group_1.begin(), group_1.end(), vector_of_genotypes[i]) != group_1.end())
{
  //For each genotype a vector with chances dead kids?
  for(int i=gs-1; i!=gs+(gs-1); ++i)
  {
    if ((chances_dead_kids_for_each_genotype[i]==0))
    {
      group_1.push_back(vector_of_genotypes[i]);
    }
  }
}
else
{
  for(int i=gs-1; i!=gs+(gs-1); ++i)
  {
    if ((chances_dead_kids_for_each_genotype[i]==0))
    {
      group_2.push_back(vector_of_genotypes[i]);
    }
  }
}

//Check all connections for genotype 1 and put them in vector "group 1"
//check if genotype 2 is in vector "group 1"
//Yes? check all connections for genotype 2 and put them in vector "group 1"
//No? check all connections for genotype 2 and put them in vector "group 2"
//Repeat for all genotypes
//Remove all double genotypes in each vector group
//Count number of vector groups to get number of "good species"
*/

//Determine number of good species from chances_dead_kids
/*
int n_good_species = 1;
for (int i=0; i!=gc; i+=1)
{


   //In this way vector_of_genotypes[1] is always considered as "good species"
  if(chances_dead_kids[i]!=0) ++n_good_species;
  if(n_good_species == gs)
*/

vector<individual> jobo::remove_inviable_species(const std::vector<individual>& individuals)
{
  assert(individuals.size()>0);
  const int sz{static_cast<int>(individuals.size())};
  std::vector<individual> viable_population;
  for (int i=0; i!=sz; i+=1)
  {
    const individual a = individuals[i];
    if (is_viable_species(a.get_genotype()))
    {
      viable_population.push_back(a);
    }
  }
  //cout << viable_population.size() << ".\n";
  assert(viable_population.size()>0);
  return viable_population;
}

int jobo::count_good_species(const std::vector<genotype>& viable_population)
{
  //cout << viable_population.size() << ".\n";
  assert(viable_population.size()>0);
  const int sz{static_cast<int>(viable_population.size())};
  if (sz == 1) return 1;
  boost::adjacency_list<
    boost::vecS, boost::vecS, boost::undirectedS, std::string
  > g;
  for (const auto genotype: viable_population)
  {
    //assert(is_viable_species(genotype));
    boost::add_vertex(genotype, g);
  }
  for (int i=0; i!=sz; ++i)
  {
    for (int j=i+1; j!=sz; ++j)
    {
      const double p{calc_chance_dead_kids(viable_population[i], viable_population[j])};
      if (p < 0.001)
      {
        const auto vip = vertices(g);
        auto from_iter = vip.first + i;
        auto to_iter = vip.first + j;
        boost::add_edge(*from_iter, *to_iter, g);
      }
    }
  }
  {
    /*
    // Create picture of all genotypes and their connections
    const std::string dot_filename{"jobo_count_good_species.dot"};
    const std::string svg_filename{"jobo_count_good_species.svg"};
    const std::string png_filename{"jobo_count_good_species.png"};
    std::ofstream f(dot_filename);
    boost::write_graphviz(f, g,
      [g](std::ostream& os, const auto iter)
      {
        os << "[label=\"" << g[iter] << "\"]";
      }
    );
    f.close();
    convert_dot_to_svg(dot_filename, svg_filename);
    convert_svg_to_png(svg_filename, png_filename);
    std::system("display jobo_count_good_species.png");
    */
  }
  //cout << count_undirected_graph_connected_components(g) << ".\n";
  return count_undirected_graph_connected_components(g);
}

int jobo::count_possible_species(const std::vector<individual>& individuals)
{
  if (individuals.empty()) return 0;
  // Ditch the duplicates to speed up the calculation
  const std::vector<genotype> z = get_unique_genotypes(individuals);
  assert(z.size()>0);
  assert(z.size()<100);
  const int sz{static_cast<int>(z.size())};
  if (sz == 1) return 0;
  boost::adjacency_list<
    boost::vecS, boost::vecS, boost::undirectedS, std::string
  > g;
  for (const auto genotype: z)
  {
    boost::add_vertex(genotype, g);
  }
  for (int i=0; i!=sz; ++i)
  {
    for (int j=i+1; j!=sz; ++j)
    {
      const double p{calc_chance_dead_kids(z[i], z[j])};
      if (p < 0.001)
      {
        const auto vip = vertices(g);
        auto from_iter = vip.first + i;
        auto to_iter = vip.first + j;
        boost::add_edge(*from_iter, *to_iter, g);
      }
    }
  }
  {
    /*
    // Create picture of all genotypes and their connections
    const std::string dot_filename{"jobo_count_possible_species.dot"};
    const std::string svg_filename{"jobo_count_possible_species.svg"};
    const std::string png_filename{"jobo_count_possible_species.png"};
    std::ofstream f(dot_filename);
    boost::write_graphviz(f, g,
      [g](std::ostream& os, const auto iter)
      {
        os << "[label=\"" << g[iter] << "\"]";
      }
    );
    f.close();
    convert_dot_to_svg(dot_filename, svg_filename);
    convert_svg_to_png(svg_filename, png_filename);
    std::system("display jobo_count_possible_species.png");
    */
  }
  return count_max_number_of_pieces(g);
}

// Note:
// It's not about how many genotypes you can shoot,
// It's about the maximum number of species you can achieve by shooting genotypes

/*
    for (int i=-1; i!=time; ++i)
    {
       individuals = connect_generations(individuals,mutation_rate,rng_engine);
       const int n_individuals{static_cast<int>(individuals.size())};
       if (n_individuals <= 1)
       {
         break;
       }
       generations = generations+1;
       set_of_genotypes = get_n_species(individuals);
       const int n_genotypes{static_cast<int>(set_of_genotypes.size())};
       if (n_genotypes != 0)
       {
          break;
       }
    }
  return set_of_genotypes;
*/

std::vector<genotype> jobo::create_test_genotypes_1()
{
  return
  {
    genotype("ab"),
    genotype("aB"),
    genotype("Ab"),
    genotype("AB")
  };
}

// Check if vector of genotypes consist incompatible genotypes
int jobo::get_n_unviable_species(
     const std::vector<genotype>& vector_of_genotypes
)
{
   // number of genotypes
   const int gsz{static_cast<int>(vector_of_genotypes.size())};
   int n_unviable_species{0};
   // loop for all genotypes
   for (int i=0; i!=gsz; ++i)
   {
    const genotype z = vector_of_genotypes[i];
    // size of genotype (again)
    const int vgsz{static_cast<int>(z.size())};
    // check if genotype is 2 or larger
    assert (vgsz >= 2);
    // loop for size of genotype-1
    for (int j=0; j < vgsz-1; j+=2)
    {
      assert (j+1 >= 1);
      assert (j+1 <= vgsz);
      if (std::islower(z[j]) && std::isupper(z[j+1])) ++n_unviable_species;
    }
   }
   return n_unviable_species;
}

  // Defenition of incompatibilities
// The standard model uses now the Gavrilets aB definition of incompatibilities
// The original AB definition of incompatibility is now an EXTRA OPTION!
// and can be found as text in the function calc_fitness (not supported by test!)

  // New version of competition
// A higher number of lowercase letters is disadvantageous for the reproduction
// chances of the individual. In this way uppercase letters are advantageous over lowercase letters
// In get_genetic_fitness the number of lowercase letters of an individual and the maximum number
// of lowercase letters (genotype size) are counted and used to make a Gauss distribution
// (so genetic fitness is between 0 and 1)

  // Threshold for incompatibilities
// A threshold for incompatibilities could be created in the extinction_low_fitness function
// However, the threshold is dependent on the number of loci in the genotype of an individual
// For genotypes with only 1,2,3,4 or 5 loci couples, the incompatibility threshold is 1.
// The implementation of an incompatibility threshold as EXTRA OPTION! with the ratio of
// 3 loci:1 incompatibility threshold can be found in the function extinction_low_fitness

  // Old Version of Competition
// The old version of competition with the effect of competition in the population on the fitness
// can be found as EXTRA OPTION! in the function goto_next_generation.
// Competition is based on the fitness of individuals: the fitness value is based on the genetic
// fitness (number of capitals in the genetic code) and population fitness (the number
// of individuals with the same genotype).
// In get_genetic_fitness the number of capitals of an individual and the maximum number of
// capitals (genotype size /2) are counted and used to make a Gauss distribution (so genetic
// fitness is between 0 and 1)
// In calc_competition the number of identical genotypes of an individual is compared to all
// other individuals and used to make a Gauss distribution (so the population fitness is between 1
// and a negative value)
// In calc_suvivability the survivability is calculated with:
// 1.0 - (comp / population_size) / fitness_gen
// (so the survivability is between 1 and a negative value)
// If the survivability is higher than the fitness threshold (0.05) for bnoth parents reproduction
// is possible

  // Time
// Now time is counted in generations and all "steps" are the same
// include time component to have differences in steps between the emergence
// of good and incipient species

  // Loci
// Maybe different mutation rate for each locus (not) necessary,
// Number of mutation rates dependent on loci

  // Ideas / problems to think about
// 1. Possibility to choose parents in "species group of genotypes",
//    and not in the entire population
//    => choosing from the entire population prevents ignoring possible compatible genotypes
// 2. The recombination step could occur with blocks of loci and not per locus
//    => for each recombination step, first create random blocks of loci, same size?
// 3. An incompatible genotype doesn't always have to lead to death,
//    possibility for a threshold of incompatible loci couples,
//    before there is an effect on viability
//    => Or lower death chance or threshold or both?
//    => Now implemented imcompatibility threshold as EXTRA OPTION!
// 4. The mutation step could occur for both parent before recombination,
//    and not in the child after recombination
//    => A mutation is more likely to occur in the reproduction process?
// 5. The mutation rate could become lower for longer existing good species groups
//    => Seems not logic that mutation rate changes because of the lifetime of a good species group
// 6. Spatial component?
// 7. Is there 1 extreme large good species group?

  // Count_incipient_species / incipient_groups
// I suggest a count_incipient_groups function to count the incipient groups:
// each of these groups would be counted as good species in the count_good_species function,
// if one or more genotypes would be removed.
// To close the gap between the BDM and the PBD model we could look at the possibilty
// to look back at previous generation to see which of the individuals
// from an incipient group were in the past counted as a good species and which individuals
// in the incipient group could be called incipient according to the PBD-model.
