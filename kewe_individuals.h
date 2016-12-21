#ifndef KEWE_INDIVIDUALS_H
#define KEWE_INDIVIDUALS_H

#include <vector>
#include "kewe_individual.h"

namespace kewe {

using individuals = std::vector<individual>;

///Creates a matrix of attractivenesses
///Lets say we have three individuals, a, b and c:
/// +---+-------------------+------------+----------------+--------------+
/// | # | female_preference | male_trait | female_ecotype | male_ecotype |
/// +---+-------------------+------------+----------------+--------------+
/// | a |       1.0         |    1.0     |       1.0      |      1.0     |
/// | b |       1.0         |    2.0     |       1.0      |      1.0     |
/// | c |       1.0         |    3.0     |       1.0      |      1.0     |
/// +---+-------------------+------------+----------------+--------------+
/// * mate_spec_mate = 1.0
/// * mate_spec_eco = 1.0
/// +-----+---------------------------+
/// | dx  | gauss(dx, 1.0)            |
/// +-----+---------------------------+
/// | 0.0 | e(-0.0) = 1.0             |
/// | 1.0 | e(-0.5) = 0.606530659713  |
/// | 2.0 | e(-2.0) = 0.135335283237  |
/// | 3.0 | e(-4.5) = 0.0111089965382 |
/// +-----+---------------------------+
///Then the matrix will be:
/// +---+---+----+----+
/// |   | a |  b |  c |
/// +---+---+----+----+
/// | a |0.0|0.61|0.14|
/// | b |1.0|0.0 |0.14|
/// | c |1.0|0.61|0.0 |
/// +---+---+----+----+
///The matrix is row-col ordered,
/// thus v[observer][observed] = v[0][1] = 0.61
///Note how the matrix prevents individuals from mating with themselves
std::vector<std::vector<double>> calc_attractivenesses(
  const individuals& pop,
  const parameters& p
);

std::vector<double> calc_competitivenesses(
  const individuals& pop,
  const parameters& p
);

///Calculate the survivabilities of the individuals in the population
std::vector<double> calc_survivabilities(
  const individuals& pop,
  const parameters& parameters
);

///Used by calc_survivabilities
std::vector<double> calc_survivabilities(
  const individuals& pop,
  const std::vector<double>& pop_comp,
  const parameters& parameters
);


} //~namespace kewe

#endif // KEWE_INDIVIDUALS_H
