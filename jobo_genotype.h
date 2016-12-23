#ifndef JOBO_GENOTYPE_H
#define JOBO_GENOTYPE_H

#include <string>

namespace jobo {

using genotype = std::string;

double calc_fitness(const genotype& g);

bool is_lowercase_character(const char c) noexcept;

bool is_valid(const genotype& g) noexcept;

bool is_uppercase_character(const char c) noexcept;

} //~namespace jobo

#endif // JOBO_GENOTYPE_H
