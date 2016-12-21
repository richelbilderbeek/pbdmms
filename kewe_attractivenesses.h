#ifndef KEWE_ATTRACTIVENESSES_H
#define KEWE_ATTRACTIVENESSES_H

#include <vector>
#include "kewe_individuals.h"
#include "kewe_attractiveness.h"
#include "kewe_fwd.h"

namespace kewe {

///The matrix is row-col ordered, thus v[observer][observed] = v[0][1] = 0.61
///The matrix is square
///The diagonals contain zeroes
using attractivenesses = std::vector<std::vector<attractiveness>>;

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
attractivenesses calc_attractivenesses(const individuals& pop,
  const simulation_parameters& p
);

bool is_valid(const attractivenesses& as) noexcept;

} //~namespace kewe
#endif // KEWE_ATTRACTIVENESSES_H
