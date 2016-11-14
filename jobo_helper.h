#ifndef JOBO_HELPER_H
#define JOBO_HELPER_H

#include <vector>
#include "jobo_individual.h"

void hello_jobo() noexcept;

int add(int a, int b) noexcept;

double divide(double a, double b);

namespace jobo {

  int count_good_species(const std::vector<individual>& population);

  std::vector<genotype> get_unique_genotypes(const std::vector<individual>& population);

} //~namespace jobo

int count_good_species(std::vector<int> chances);

#endif // JOBO_HELPER_H
