#include "jobo_individual.h"
#include "jobo_parameters.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <cassert>
#include <cctype>

jobo::individual::individual(const std::string& genotype
) : m_genotype{genotype},m_fitness{0}
{
}

int jobo::calc_fitness(std::string genotype)
{
  if (genotype.size() % 2 == 1)
  {
    throw std::invalid_argument("genotype length must be even");
  }

  //check for each 2 characters of genotype if both letters are uppercase, then fitness = 0
  const int sz{static_cast<int>(genotype.length())};
  for (int i=0; i!=sz; i+=2)
  {
    const char a{genotype[i+0]};
    const char b{genotype[i+1]};
    if (std::isupper(a) && std::isupper(b)) return 0;
  }
  //at least one lower case letter in the two genotype cahracters (aB,Ab or ab so fitness = 1)
  return 1;
}

//SECOND ATTEMPT FOR OFFSPRING

jobo::individual create_offspring(const jobo::individual mother,const jobo::individual father)
{
  //Trying to create offspring individual
  jobo::individual& kid = jobo::individual mother;
  //Trying to give kid recombinated genotype of parents
  jobo::individual kid {recombine(p,q)};
  //Creates a kid from random recombination of parents' genotype
  //Will throw if genotypes are of different lengths
    if (p(genotype.size()) =! q(genotype.size()))
    {
      throw std::invalid_argument("genotype length must be even");
    }
   return jobo::individual kid;
}

jobo::individual::genotype recombine(const genotype& p, const genotype& q)
{
  //Randomly combines genotypes p and q
  //Make genotype of kid (r) same as genotype parent (q)
  genotype& r = q;
  //Make loop to include all loci
  const int sz{static_cast<int>(genotype.length())};
  for (int i=0; i!=sz; i+=1)
    {
    //draw random number
    n = rand() % 100;
    //check if number is even or odd
    if (n % 2 == 1)
      {
      genotype r[i] = genotype {q[i]};
      }
    else genotype r[i] = genotype {p[i]};
//Will throw if genotypes are of different lengths
    if (p (genotype.size()) =! q (genotype.size()))
    {
      throw std::invalid_argument("genotype length must be even");
    }
  return genotype (p,q);
}

/*
 * FIRST ATTEMPT FOR OFFSPRING
//structure of individual create_offspring
//need two parents with their own genotype string
jobo::individual create_offspring(std::string genotype)
{
   //create_offspring needs to use both different parents genotype (of 6 loci)
   //declare offspring (as part of the individual class?)

   parent_1 = jobo::individual(genotype);
   parent_2 = jobo::individual(genotype);

   //create loop with number of loci
   const int sz{static_cast<int>(genotype.length())};
   for (int i=0; i!=sz; i+=1)
   {
      //create chance with random bool for each parent to use its locus
      bool locus_chance;

      //use locus of "winning" parent
      //if locus_chance == 1, parent_1 locus will be used
      if(locus_chance == 1)
      offspring[i] = parent_1[i];

      //if locus_chance == 0, parent_2 locus will be used
      else offspring[i] = parent_2[i];
   }
   return offspring;
}
*/

//MUTATION THEORY

// create_mutated_offspring needs to (create difference in)/replace one locus with the locus of opposite size
// in the offspring genotype string (the copy/outcome of the parent(s) genotype strings).
// The parameter mutation rate (for each locus, but with same rate?, dependent on number of loci)
// will be used to determine if mutation will happen.

//mutaton rate per locus seems necessary but not really usefull...? example mutation_rate_locus_aA

/*
 //implement following mutation step in create_offspring
std::string& create__mutated_offspring(std::string genotype)
{
  if (mutation_rate_locus_aA == 1)
    {
    // mutate locus a to locus A in genotype abcdef
    genotype.replace(genotype.begin(), genotype.end(), 'a', 'A');
    }
  if (mutation_rate_locus_bB == 1)
    {
    // mutate locus b to locus B in genotype abcdef
    genotype.replace(genotype.begin(), genotype.end(), 'b', 'B');
    }
  if (mutation_rate_locus_cC == 1)
    {
    // mutate locus c to locus C in genotype abcdef
    genotype.replace(genotype.begin(), genotype.end(), 'c', 'C');
    }
  return genotype;
}
*/

bool jobo::operator==(const individual& lhs, const individual& rhs) noexcept
{
  return lhs.get_genotype() == rhs.get_genotype();
}

bool jobo::operator!=(const individual& lhs, const individual& rhs) noexcept
{
  return !(lhs == rhs);
}
