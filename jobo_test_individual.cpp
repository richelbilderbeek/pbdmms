#include "jobo_test_individual.h"
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

<<<<<<< HEAD
/*
  //Fitness calculation for uneven genotypes
  {
   if (calc_fitness("a") !=1.0) ++n_fails;
   if (calc_fitness("abc") !=0.0) ++n_fails;
   if (calc_fitness("abcde") !=0.0) ++n_fails;
   if (calc_fitness("AbCDe") !=0.0) ++n_fails;
   if (calc_fitness("abCDE") !=0.0) ++n_fails;
   if (calc_fitness("abC") !=0.0) ++n_fails;
   if (calc_fitness("AbC") !=0.0) ++n_fails;
  }
*/
=======

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
>>>>>>> a05e699d2859bc54435d3a990d8704ba4364f3c0

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

  return n_fails;
}
