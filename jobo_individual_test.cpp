#include "jobo_individual_test.h"
#include <cassert>
#include <string>
#include <stdexcept>
#include <iostream>
#include "jobo_individual.h"

int jobo::individual_test() noexcept
{
  int n_fails{0};

  //An individual has a genotype
  {
    const std::string genotype("ab");
    const individual i(genotype);
    if (i.get_genotype() != genotype) ++n_fails;
  }

  //A genotype has a number of loci
  {
    const int n_loci{2};
    const std::string genotype("ab");
    assert(n_loci == static_cast<int>(genotype.size()));
    const individual i(genotype);
    if (i.get_n_loci() != static_cast<int>(genotype.size())) ++n_fails;
  }

  //A copy of an individual is identical
  {
    const std::string genotype("ab");
    const individual i(genotype);
    const individual j(i);
    if (i != j) ++n_fails;
  }

  //Fitness calculation of genotype
  {
    if (calc_fitness("ab") != 1.0) ++n_fails;
    if (calc_fitness("Ab") != 1.0) ++n_fails;
    if (calc_fitness("aB") != 1.0) ++n_fails;
    if (calc_fitness("AB") != 0.0) ++n_fails;
  }

  //Fitness calculation of genotype with 4 characters
  {
    if (calc_fitness("abcd") != 1.0) ++n_fails;
    if (calc_fitness("AbCd") != 1.0) ++n_fails;
    if (calc_fitness("aBcD") != 1.0) ++n_fails;
    if (calc_fitness("AbcD") != 1.0) ++n_fails;
    if (calc_fitness("abCD") != 0.0) ++n_fails;
    if (calc_fitness("ABcd") != 0.0) ++n_fails;
  }

  //Fitness calculation of genotype with 6 characters
  {
    if (calc_fitness("abcdef") !=1.0) ++n_fails;
    if (calc_fitness("AbCdEf") !=1.0) ++n_fails;
    if (calc_fitness("aBcDeF") !=1.0) ++n_fails;
    if (calc_fitness("ABcdef") !=0.0) ++n_fails;
    if (calc_fitness("abCDef") !=0.0) ++n_fails;
    if (calc_fitness("abcdEF") !=0.0) ++n_fails;
  }

  //Fitness calculation for genotypes of odd lengths should throw an exception
  {
    ++n_fails;
    try
    {
      calc_fitness("abc");
    }
    catch (std::invalid_argument&)
    {
      --n_fails;
    }
  }

  //Recombine
  {
    std::mt19937 rng_engine(42);
    const genotype p("abcdefghijklmnopqrstuvwxyz");
    const genotype q("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    assert(p == p);
    assert(p != q);
    const genotype r = recombine(p, q, rng_engine);
    if (r == p) ++n_fails;
    if (r == q) ++n_fails;
  }

  //Create_offspring with recombined genotype
  {
    std::mt19937 rng_engine(42);
    const genotype p("abcdefghijklmnopqrstuvwxyz");
    const genotype q("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    const individual mother(p);
    const individual father(q);
    const individual child = create_offspring(mother,father,rng_engine);
    if (child == mother) ++n_fails;
    if (child == father) ++n_fails;
    const genotype r = child.get_genotype();
    if (r == p) ++n_fails;
    if (r == q) ++n_fails;
  }

   //mutation_check_all_loci
  {
    std::mt19937 rng_engine(42);
    const double mutation_rate (0.5);
    const genotype r("ABCDEFGHIJKLmnopqrstuvwxyz");
    const genotype v = mutation_check_all_loci(r,mutation_rate,rng_engine);
    //const individual before_mutation(r);
    //const individual after_mutation = mutation_check_all_loci(r,mutation_rate,rng_engine);
    if (r == v) ++n_fails;
  }

  //Create individual with mutation with create_mutation
  {
   std::mt19937 rng_engine(42);
   const double mutation_rate (0.5);
   const genotype r("ABCDEFGHIJKLmnopqrstuvwxyz");
   const individual before_mutation(r);
   const individual after_mutation = create_mutation(before_mutation,mutation_rate,rng_engine);
   if (before_mutation == after_mutation) ++n_fails;
  }

 return n_fails;
}


/*
  //Change genotype with mutation function
  {
  const genotype p("abcdefghijklmnopqrstuvwxyz");
  const genotype s = mutation_one_locus(p);
  if (s == p) ++n_fails;
  }
*/

//1
  //create offspring with copy of genotype of (one of) the parents CHECK
  //create offspring as "mix" of parents CHECK

//2
  //create mutation rate parameter (per locus?)
  //check for one mutation event/possibility (in 1 locus) in offspring
  //Offspring may have a different genotype

//3  
  //check offspring as mix parents plus effects mutation(s)
  //(Backward) mutation is possible!

//4
  //extinction at low fitness; no offspring for fitness 0
  //extra extinction rate independent of genotype?

//5
  //create "time/mutation steps"
  //multiple mutations/generations possible
  //backward mutation?

//6
  //label "incipient" and "good" species
  //label crown age
  //label speciation completion event in time
  //label incipient origin event in time
  //read number of species/ lineages through time

//7 (Not necessary for research question)
  //create/visualize tree
  //create/visualize tree without "incipient" species

