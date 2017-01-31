#ifndef KEWE_ATTRACTIVENESS_H
#define KEWE_ATTRACTIVENESS_H

#include "kewe_fwd.h"
#include "kewe_gausser.h"

namespace kewe {

///attractiveness is zero or more
using attractiveness = double;

///Equation 1 in Van Doorn & Weissing 2001
/// @param female_preference
/// @param male_trait
/// @param mate_spec_mate mate choice specificity with respect to mate choice, sigma_m in Table 1
/// @param female_ecotype
/// @param male_ecotype
/// @param mate_spec_eco mate choice specificity with respect to ecological type, sigma_e in Table 1
attractiveness calc_attractiveness(
  const double female_preference,
  const double male_trait,
  const gausser& gauss_mate_spec_mate,
  const double female_ecotype,
  const double male_ecotype,
  const gausser& gauss_mate_spec_eco
) noexcept;


attractiveness calc_attractiveness(
  const individual& female,
  const individual& male,
  const simulation_parameters& parameters
);

attractiveness calc_attractiveness(
  const individual& female,
  const individual& male,
  const gausser& gauss_mate_spec_mate,
  const gausser& gauss_mate_spec_eco
);

} //~namespace kewe

#endif // KEWE_ATTRACTIVENESS_H
