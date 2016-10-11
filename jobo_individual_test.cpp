#include "jobo_individual_test.h"
#include "jobo_individual.h"
#include <string>

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
  /* TODO
  {
    const std::string genotype("ab");
    const individual i(genotype);
    if (i.get_n_loci() != genotype.size()) ++n_fails;
  }
  */

  //A copy of an individual is identical
  /*  TODO
  {
    const std::string genotype("ab");
    const individual i(genotype);
    const individual j(i);
    if (i != j) ++n_fails;
  }
  */

  //Offspring may have a different genotype
  /* TODO
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
