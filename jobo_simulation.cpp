#include "jobo_simulation.h"
#include "jobo_parameters.h"
#include "jobo_individuals.h"
#include "jobo_individual.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
#include <cstdio>
#include <cctype>
#include <string>
#include <stdexcept>
#include <random>
#include <boost/graph/adjacency_list.hpp>
#include "count_undirected_graph_connected_components.h"

using namespace std;
using namespace jobo;

jobo::simulation::simulation(
  const parameters& parameters
) noexcept
  : m_individuals(parameters.get_population_size()),
    m_parameters{parameters},
    m_rng_engine(parameters.get_seed())
{
}

std::vector<int> jobo::get_random_ints(std::mt19937& rng_engine, int n)
{
  //Need to use number of loci to get number of random int with 1 seed
  std::vector<int> n_loci_ints;
  n_loci_ints.resize(n);

  std::uniform_int_distribution<int> distribution(0,100);
  for (int i=0; i!=n; ++i)
  {
    int w = distribution(rng_engine);
    n_loci_ints[i] =  w;
  }
 //Need to get all random int in one return value
 return n_loci_ints;
}

std::vector<double> jobo::get_random_doubles(std::mt19937& rng_engine, int n)
{
  //Need to use number of loci to get number of random doubles with 1 seed
  std::vector<double> n_loci_doubles;
  n_loci_doubles.resize(n);

  std::uniform_real_distribution<double> distribution(0,1);
  for (int i=0; i!=n; ++i)
  {
    double w = distribution(rng_engine);
    n_loci_doubles[i] =  w;
  }
 //Need to get all random doubles in one return value
 return n_loci_doubles;
}

std::vector<int> jobo::get_random_parents(
  std::mt19937& rng_engine,
  int population_size
)
{
  std::vector<int> get_random_parents;
  const int number_of_parents{200};
  //const int number_of_parents = population_size*2;
  get_random_parents.resize(number_of_parents);
  std::uniform_int_distribution<int> distribution(0,population_size-1);
  for (int i=0; i!=number_of_parents; ++i)
    {
    int w = distribution(rng_engine);
    get_random_parents[i] =  w;
    }
  //TODO prevent that parents are same individual
  return get_random_parents;
}

std::vector<individual> jobo::goto_next_generation(
  std::vector<individual> individuals,
  const double mutation_rate,
  std::mt19937& rng_engine
)
{
  const int population_size{static_cast<int>(individuals.size())};

  //Get random numbers to select random individuals
  const std::vector<int> random_parents = get_random_parents(rng_engine, population_size);
  const int n_couples{static_cast<int>(random_parents.size()) / 2};
  std::vector<individual> new_individuals;

  //Repeat create_offspring by the number of couples
  for (int i=0; i!=n_couples; ++i)
  {
    //Get random father, pick random individual from vector
    const int number_father = random_parents[i];
    //assert(number_father < static_cast<int>(individuals.size()));
    const individual father = individuals[number_father];
    //Get random mother, pick random individual from vector
    //assert(i+population_size < random_parents.size());
    const int number_mother = random_parents[i+n_couples];
    //assert(number_mother < static_cast<int>(individuals.size()));
    const individual mother = individuals[number_mother];
    //Create kid
    const individual offspring = create_offspring(mother, father, rng_engine);
    if (offspring.get_genotype().size() % 2 != 0)
    {
      throw std::invalid_argument("genotype length must be even");
    }
    new_individuals.push_back(offspring);
  }
  //Loop through every individual of new_individuals to check for mutation(s)
  for (int i=0; i!=population_size; ++i)
  {
    //Use create_mutation for genotype of each individual
    new_individuals[i] = create_mutation(new_individuals[i],mutation_rate,rng_engine);
  }
  const int n_new_individuals{static_cast<int>(new_individuals.size())};
  assert(n_new_individuals == 100);
  return new_individuals;
}

std::vector<individual> jobo::extinction_low_fitness(
  std::vector<individual> new_individuals
)
{
  //Loop through every individual of new_individuals to check fitness level
  const int population_size{static_cast<int>(new_individuals.size())};
  std::vector<int> fitness_levels;
  std::vector<individual> living_individuals;
  living_individuals = new_individuals;

  for (int i=0; i!=population_size; ++i)
  {
    const individual k = new_individuals[i];
    //Use calc_fitness to get fitness level of each genotype (genotype needed)
    assert(k.get_genotype().size() % 2 == 0);
    int n_low_fitness = calc_fitness(k.get_genotype());
    //make vector of fitness levels for each (new)individual
    fitness_levels.push_back(n_low_fitness);
  }

  const int f{static_cast<int>(fitness_levels.size()-1)};
  for (int i=f; i!=-1; --i)
  {
    if (fitness_levels[i] != 1)
    {
      living_individuals.erase(living_individuals.begin()+i);
      fitness_levels.erase(fitness_levels.begin()+i);
    }
  }

  const int g{static_cast<int>(fitness_levels.size()-1)};
  for (int i=g; i!=-1; --i)
  {
  assert(fitness_levels[i] != 0);
  }

  const int living_size{static_cast<int>(living_individuals.size())};
  for (int i=0; i!=living_size; ++i)
    {
      const individual w = living_individuals[i];
      assert(w.get_genotype() != "ABCDEF");
    }
  //Use fitness vector to remove individual(s) from new_individuals
  return living_individuals;
}

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

std::vector<genotype> jobo::get_unique_genotypes(
    std::vector<individual> individuals
)
{
  const int population_size{static_cast<int>(individuals.size())};
  // run through population to collect all genotypes
  std::set<std::string> set_of_genotypes;
  for (int i=0; i!=population_size; ++i)
  {
    // Create set function to store all unique genotypes
    // at least including the parental genotype of generation 1
    const individual w = individuals[i];
    set_of_genotypes.insert(w.get_genotype());
  }
  //change set_of_genotypes into vector_of_genotypes
  vector<string>vector_of_genotypes(vector_of_genotypes.begin(),vector_of_genotypes.end());

  //return set with all unique genotypes
  return vector_of_genotypes;
}

// Calculate the chance of dead offpsring of two genotypes
double jobo::calc_chance_dead_kids(
    genotype w, genotype q
)
{
//test if both genotypes have same size
 assert(w.size() == q.size());
 const int wz{static_cast<int>(w.size())};
 vector<double> chs_dead_offspring ;
  for (int i=0; i!=wz; i+=2)
  {
    //test if both first loci are lower case letters = 0
    double ch_dead_offspring;
    if(w[i] == q[i])
    {
      ch_dead_offspring = 0;
    }
    //test if both second loci are lower case letters = 0
    if (w[i+1] == q[i+1])
    {
      ch_dead_offspring = 0;
    }
    //in other cases the chance to die for the loci couple = 0,25
    else
    ch_dead_offspring = 0.25;
   chs_dead_offspring.push_back(ch_dead_offspring);
  }

  //calculate the chance of dead offspring for all loci together
  double chance_dead_kids = 0;
  std::for_each(chs_dead_offspring.begin(), chs_dead_offspring.end(),
  [&] (double n) {
    chance_dead_kids += n;
  });
  chance_dead_kids = chance_dead_kids/(w.size()/2);

  //return chance (double) of dead offspring for the 2 genotypes
  return chance_dead_kids;
}


int jobo::count_good_species(const std::vector<individual> individuals)
{
  if (individuals.empty()) return 0;

  //Ditch the duplicates to speed up the calculation
  const std::vector<genotype> z = get_unique_genotypes(individuals);

  const int sz{static_cast<int>(z.size())};
  if (sz == 1) return 1;

  boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> g(sz);

  for (int i=0; i!=sz; ++i)
  {
    for (int j=i+1; j!=sz; ++j)
    {
      const double p{calc_chance_dead_kids(z[i], z[j])};
      if (p < 0.0001)
      {
        const auto vip = vertices(g);
        auto from_iter = vip.first + i;
        auto to_iter = vip.first + j;
        boost::add_edge(*from_iter, *to_iter, g);
      }
    }
  }
  return count_undirected_graph_connected_components(g);

}


/*
// Determine the number of good species
std::vector<double> jobo::get_chances_dead_kids(
    std::set<genotype> vector_of_genotypes
)
{
//change vector_of_genotypes into vector_of_genotypes
 std::vector<std::string> vector_of_genotypes(vector_of_genotypes.begin(), vector_of_genotypes.end())
     ;
//create loop to get all couples of unique genotypes, to calculate for each couple
//the chance of dead offspring
const int gs{static_cast<int>(vector_of_genotypes.size())};

std::vector<double> chances_dead_kids;
for(int i=0; i!=gs; ++i)
{
  for(int j=i+1; j!=gs; ++j)
  {
    assert(i < j);
    double chance_dead_kids = calc_chance_dead_kids(vector_of_genotypes[i],vector_of_genotypes[j]);
    //store all chances for dead offspring for all genotype couples
    chances_dead_kids.push_back(chance_dead_kids);
  }
}


std::vector<double> chances_dead_kids;
for(int i=0; i!=gs; ++i)
{
  for(int j=0; j!=gs; ++j)
  {
    double chance_dead_kids = calc_chance_dead_kids(vector_of_genotypes[i],vector_of_genotypes[j]);
    chances_dead_kids.push_back(chance_dead_kids);
  }
}
return chances_dead_kids;
}

int jobo::get_n_good_species(
    std::vector<double> chances_dead_kids,
    std::set<genotype> vector_of_genotypes
)
{
std::vector<std::string> vector_of_genotypes(vector_of_genotypes.begin(), vector_of_genotypes.end());
const int gs{static_cast<int>(vector_of_genotypes.size())};
//const int gc{static_cast<int>(chances_dead_kids.size())};
std::vector<std::string> group_1{vector_of_genotypes[1]};
int n_good_species{1};

//Check all connections for genotype 1 and put them in vector "group 1"
//Create group_1 vector including the first genotype
//Set up first group of genotypes of the same "good species"
for(int i=0; i!=gs-1; ++i)
{
  //Add all genotypes connected to genotype 1
  if (chances_dead_kids[i]==0)
  {
  group_1.push_back(vector_of_genotypes[i]);
  }
}

//Check next genotype
//Repeat for all genotypes
  for(int i=1; i!=gs; ++i)
//check if genotype 2 is in vector "group 1"
//Check if genotype is already part of group
//1. TODO now only checking for first group
  {

    if(std::find(group_1.begin(), group_1.end(), vector_of_genotypes[i]) != group_1.end())
    {
    //Yes? check all connections for genotype 2 and put them in vector "group 1"
    //Look at only the chances_dead_kids_for_each_genotype for 1 genotype
      for(int j=(i*gs)-1; j!=(i*gs)+(gs-1); ++j)
      {
        //Add all genotypes connected to the checked genotype to group 1
        if (chances_dead_kids[j]==0)
        {
          group_1.push_back(vector_of_genotypes[j-(i*gs)]);
        }
      }
    }
    else
    {
      //No? check all connections for genotype i and put them in vector "group i"
      for(int k=(i*gs)-1; k!=(i*gs)+(gs-1); ++k)
      {
        //Add all genotypes of
        if (chances_dead_kids[k]==0)
        {
          //Create a new group to store genotype and possible connected genotypes
          //2. TODO create group for each genotype with own number
          std::vector<std::string> group_2;
          group_2.push_back(vector_of_genotypes[k-(i*gs)]);
          //Count the new group as a new good species
          n_good_species = n_good_species +1;
        }
      }
    }
  }
  return n_good_species;
}
//Not neccesary:
//Remove all double genotypes in each vector group
//Count number of vector groups to get number of "good species"

//Too simple, not working for "bridge between groups"
int jobo::get_n_incipient_species(
   std::vector<double> chances_dead_kids,
   std::set<genotype> vector_of_genotypes
)
{
  std::vector<std::string> vector_of_genotypes(vector_of_genotypes.begin(), vector_of_genotypes.end());
  const int gs{static_cast<int>(vector_of_genotypes.size())};
  int n_incipient_species{0};
  int n_connections{0};
  for(int i=1; i!=gs; ++i)
  {
    for(int j=(i*gs)-1; j!=(i*gs)+(gs-1); ++j)
    {
      //Check if there is a genotype with only one connection

       if (chances_dead_kids[j]==0)
       {
         n_connections = n_connections+1;
       }
    }
  }
  //for each genotype with one connection, n_incipient_species++1
  if (n_connections == 1)
  n_incipient_species = n_incipient_species+1;
return n_incipient_species;
}

*/

//Create test population for tests
std::vector<genotype> jobo::create_test_population_1(
    int time
)
{

    const double mutation_rate (0.5);
    int generations (0);
    std::mt19937 rng_engine(42);
    std::vector<individual> individuals(100, individual("abcdefgh"));
    std::vector<genotype> vector_of_genotypes;

    for (int i=-1; i!=time; ++i)
    {
       individuals = connect_generations(individuals,mutation_rate,rng_engine);
       const int n_individuals{static_cast<int>(individuals.size())};
       if (n_individuals <= 1)
       {
         break;
       }
       generations = generations+1;
       vector_of_genotypes = get_unique_genotypes(individuals);
       const int n_genotypes{static_cast<int>(vector_of_genotypes.size())};
       if (n_genotypes != 0)
       {
          break;
       }
    }
  return vector_of_genotypes;
}

int jobo::get_n_unviable_species(
     std::vector<genotype> vector_of_genotypes
)
{
   genotype ab = vector_of_genotypes[1];
   const int sz{static_cast<int>(ab.size())};
   const int gsz{static_cast<int>(vector_of_genotypes.size())};
   int n_unviable_species{0};
   for (int i=0; i!=gsz; ++i)
   {
    const genotype z = vector_of_genotypes[i];
    for (int i=0; i!=sz; i+=2)
    {
      const char a{z[i+0]};
      const char b{z[i+1]};
      if (std::isupper(a) && std::isupper(b)) ++n_unviable_species;
    }
   }
   return n_unviable_species;
}

//First "results function"  with only cout output
void jobo::create_output_with_cout(
    int time,
    double mutation_rate,
    std::mt19937 rng_engine,
    int generations,
    std::vector<individual> individuals
    )
{
    int generation{0};
    std::vector<genotype> vector_of_genotypes;
    std::cout << "Generation: "<< generation << '\n';
    std::cout << "Number of individuals: " << individuals.size() << '\n' << '\n';
    for (int i=0; i!=time; ++i)
    {
      generations = generations+1;
      individuals = connect_generations(individuals,mutation_rate,rng_engine);

      //Show extinction process of the populations
      std::cout << "Generation: " << generations << '\n';
      std::cout << "Number of individuals after extinction: " << individuals.size() << '\n';

      //Stop simulation if population size is 1
      if (individuals.size() == 1)
      {
        break;
      }

      //Count genotypes
      vector_of_genotypes = get_unique_genotypes(individuals);
      //std::vector<double>  chances_dead_kids = get_chances_dead_kids(vector_of_genotypes);
      int n_species = static_cast<int>(vector_of_genotypes.size());
      int n_good_species = count_good_species(individuals);
      //int n_incipient_species = get_n_incipient_species(chances_dead_kids,vector_of_genotypes);

      //Show number of species, good species and incipient species
      std::cout << "Number of species: " << n_species << '\n';
      std::cout << "Number of 'good' species: " << n_good_species << '\n';
      //std::cout << "Number of 'incipient' species: " << n_incipient_species << '\n' <<  '\n';
    }
    return;
}

      // Visualization
    // Now output is created with create_output_with_cout,
    // including generation, individuals, species, good species and incipient species
    // # Visualize different generations in phylogenetic tree/lineages through time plot
    // # with create_output

      //Time
    // Now time is counted in generations and all "steps" are the same
    // # include time component to have differences in steps between the emergence
    // of good and incipient species

      // Loci
    // Maybe different mutation rate for each locus (not) necessary,
    // Number of mutation rates dependent on loci
    // Make it impossible for individual to have 1 individual as parents
    // # create possibility to have more than 26 loci
    // # create code with letter and number



