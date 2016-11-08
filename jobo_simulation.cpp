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
  const int number_of_parents = population_size*2;
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
  //assert (population_size > 1);
  //Get random numbers to select random individuals
  const std::vector<int> random_parents = get_random_parents(rng_engine, population_size);
  const int n_couples{static_cast<int>(random_parents.size()) / 2};
  assert(n_couples == population_size);

  std::vector<individual> new_individuals;

  //Repeat create_offspring by the number of couples
  for (int i=0; i!=n_couples; ++i)
  {
    //Get random father, pick random individual from vector
    const int number_father = random_parents[i];
    assert(number_father < static_cast<int>(individuals.size()));
    const individual father = individuals[number_father];
    //Get random mother, pick random individual from vector
    //assert(i+population_size < random_parents.size());
    const int number_mother = random_parents[i+population_size];
    assert(number_mother < static_cast<int>(individuals.size()));
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
  return new_individuals;
}

int jobo::update_generations(int generations
)
{
//TODO remove oclint error: parameter reassignment
generations = generations+1;
return generations;
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
std::set<genotype> jobo::get_n_species(
    std::vector<individual> individuals
)
{
  const int population_size{static_cast<int>(individuals.size())};
  // make vector to store all unique genotypes: genotypes_species
  //std::vector<std::string> genotypes_species(1, );
  // run through population to collect all genotypes
  std::set<std::string> set_of_genotypes;
  for (int i=0; i!=population_size; ++i)
  {
    // Create set function to store all unique genotypes
    // at least including the parental genotype of generation 1
    const individual w = individuals[i];
    set_of_genotypes.insert(w.get_genotype());
  }

  //std::cout<< "Number of different genotypes = " << set_of_genotypes.size() << std::endl;
  // Iterate through all genotypes in the set and display the loci.
  //for (std::set<std::string>::iterator it=set_of_genotypes.begin();
  //it!=set_of_genotypes.end(); ++it)
  //std::cout << ' '  <<*it << '\n';

  //return set with all unique genotypes
  return set_of_genotypes;
}

// Calculate the chance of dead offpsring of two genotypes
double jobo::calc_chance_dead_kids(
    genotype w, genotype q
)
{
//test if both genotypes have same size
 assert(w.size() == q.size());
 const int wz{static_cast<int>(w.size())};
 std::vector<double> chs_dead_offspring ;
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

// Determine the number of good species
int jobo::get_n_good_species(
    std::set<genotype> set_of_genotypes
)
{
//change set_of_genotypes into vector_of_genotypes
 std::vector<std::string> vector_of_genotypes(set_of_genotypes.begin(), set_of_genotypes.end());
//create loop to get all couples of unique genotypes, to calculate for each couple
//the chance of dead offspring
const int gs{static_cast<int>(vector_of_genotypes.size())};
std::vector<double> chances_dead_kids;
for(int i=0; i!=gs;++i)
{
  for(int j=i+1; j!=gs; ++j)
  {
    assert(i < j);
    double chance_dead_kids = calc_chance_dead_kids(vector_of_genotypes[i],vector_of_genotypes[j]);
    //store all chances for dead offspring for all genotype couples
    chances_dead_kids.push_back(chance_dead_kids);
  }
}

const int gc{static_cast<int>(chances_dead_kids.size())};
//Determine number of good species from chances_dead_kids
int n_good_species = 1;
for (int i=0; i!=gc; i+=1)
{
  if(chances_dead_kids[i]!=0) ++n_good_species;\
  if (n_good_species == gs)
    {
      break;
    }
}
if(n_good_species ==gs)
{
  for (int i=gc-1; i>(gs-2); i--)
  {
    if(chances_dead_kids[i]==0) --n_good_species;
    if (n_good_species == 2)
    {
      break;
    }
  }
}
return n_good_species;
}

//
int jobo::get_n_incipient_species (
   int n_good_species,
   std::set<genotype> set_of_genotypes
)
{
int n_genotypes{static_cast<int>(set_of_genotypes.size())};
int n_incipient_species = n_genotypes - n_good_species;
return n_incipient_species;
}


//Create test population for tests
std::set<genotype> jobo::create_test_population_1(
    int time
)
{
    std::set<genotype> set_of_genotypes;
    const double mutation_rate (0.5);
    int generations (0);
    std::mt19937 rng_engine(42);
    std::vector<individual> individuals(100, individual("abcdefgh"));

    for (int i=0; i!=time; ++i)
    {
       individuals = connect_generations(individuals,mutation_rate,rng_engine);
       const int n_individuals{static_cast<int>(individuals.size())};
       if (n_individuals <= 1)
       {
         break;
       }       
       generations = update_generations(generations);
       set_of_genotypes = get_n_species(individuals);
       const int n_genotypes{static_cast<int>(set_of_genotypes.size())};
       if (n_genotypes != 0)
       {
          break;
       }
       //int n_good_species = get_n_good_species(set_of_genotypes);
       //int n_incipient_species = get_n_incipient_species(n_good_species,set_of_genotypes)
    }
  return set_of_genotypes;
}

int jobo::get_n_unviable_species(
     std::set<genotype> set_of_genotypes
)
{
   std::vector<std::string> vector_of_genotypes(set_of_genotypes.begin(), set_of_genotypes.end());
   genotype ab = vector_of_genotypes[1];
   const int sz{static_cast<int>(ab.size())};
   const int gsz{static_cast<int>(set_of_genotypes.size())};
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


      // Visualization
    // Visualize different generations in tree with number of individuals,
    // generation number, number of genotypes
    //(number of good and incipient species for each generation)
    // (Determine speciation completion rate)

      // Good and incipient
    // divide genotypes between incipient and good species:
    // use chance for kid to die without mutations
    //(so only if two individuals create offspring without mutation step)
    // to distinguish species (for example: if fitness is lower, new species)
    // Give each loci couple code with 0 for lower case letter and
    // 1 for capital letter to calculate chance for dead kid for each loci couple
    // HOW TO COMPARE 2 GENOTYPES FOR EACH LOCI-COUPLE?

    //a & a = 0
    //b & b = 0

    //A,a,b,B = 0,25
    //a,A,B,b = 0,25

    //Not possible, parent should be dead:
    //a,A,B,B = 0,5
    //A,a,B,B = 0,5
    //A,A,B,b = 0,5
    //A,A,b,B = 0,5
    //A,A,B,B = 1
    //a,A,b,B = 0,25

      // Extra birth rate
    // compensate extinction incompatibles with birth,
    // create extra offspring to compensate the percentage of died incompatibles
    // HOW TO INPLEMENT THE DYING INCOMPATIBLES INTO OFFSPRING LOOP?

      // Loci
    // Maybe different mutation rate for each locus (not) necessary
    // Number of mutation rates dependent on loci
    // create possibility to have more than 26 loci
    // CREATE CODE WITH LETTER AND NUMBER?



