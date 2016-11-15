#ifndef JOBO_HELPER_H
#define JOBO_HELPER_H

#include <vector>
#include "jobo_individual.h"

void hello_jobo() noexcept;

int add(int a, int b) noexcept;

double divide(double a, double b);

int count_good_species(std::vector<int> chances);

#endif // JOBO_HELPER_H
