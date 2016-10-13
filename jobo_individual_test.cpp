#include "jobo_individual_test.h"
#include "jobo_individual.h"
#include <cassert>
#include <string>
#include <stdexcept>

int jobo::test_individual() noexcept
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

  //Offspring is identical to parents
  //create individual "offspring" with the genotype created with the create_offspring() function
  //TO DO: with the use of both parents genotype!

  {
    const std::string genotype("abcdef");
    const individual i(genotype);
    const individual j(i);
    if (i != j) ++n_fails;
    const individual offspring (i.create_offspring());
    if (offspring != i) ++n_fails;
    if (offspring != j) ++n_fails;
  }

  {
    const std::string genotype("abcdef");
    const individual i(genotype);
    const individual j(i);
    const individual offspring (i.create_offspring());
    if (offspring != i) ++n_fails;
    if (offspring != j) ++n_fails;
  }

  {
    const std::string genotype("abcdef");
    const individual i(genotype);
    const individual j(i);
    const individual offspring (i.create__mutated_offspring());
    if (offspring == i) ++n_fails;
    if (offspring == j) ++n_fails;
  }

  return n_fails;
}

//1
  //create offspring with copy of genotype of (one of) the parents

//2
  //create mutation rate parameter (per locus?)
  //check for one mutation event/possibility (in 1 locus) in offspring

  //Offspring may have a different genotype
  /*
  {
    //Create parents
    const std::string genotype("ab");
    const individual i(genotype);
    const individual j(i);
    const individual kid = create_offsping(i, j, mutation_rate_per_locus);
  }
   */

//3
  //create offspring as "mix" of parents
  //check offspring as mix plus mutation for


//4
  //extinction at low fitness; no offspring for fitness 0

//5
  //create "time/mutation steps"
  //multiple mutations/generations possible
  //backward mutation?

//6
  //label "incipient" and "good" species
  //label crown  age
  //label speciation completion event in time
  //label incipient origin event in time

//7
  //create/visualize tree
  //create/visualize tree without "incipient" species


