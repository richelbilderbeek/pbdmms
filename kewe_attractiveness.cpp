#include "kewe_attractiveness.h"

#include <cassert>

#include "kewe_helper.h"
#include "kewe_individual.h"
#include "kewe_simulation_parameters.h"
#include "gausser.h"

kewe::attractiveness kewe::calc_attractiveness(
  const double female_preference,
  const double male_trait,
  const ribi::gausser& gauss_mate_spec_mate,
  const double female_ecotype,
  const double male_ecotype,
  const ribi::gausser& gauss_mate_spec_eco
) noexcept
{
  return gauss_mate_spec_mate(female_preference - male_trait)
       * gauss_mate_spec_eco(female_ecotype - male_ecotype);
}

kewe::attractiveness kewe::calc_attractiveness(
  const double female_preference,
  const double male_trait,
  const double mate_spec_mate,
  const double female_ecotype,
  const double male_ecotype,
  const double mate_spec_eco
)
{
  assert(mate_spec_mate >= 0.0);
  assert(mate_spec_eco >= 0.0);
  return gauss(female_preference - male_trait, mate_spec_mate)
       * gauss(female_ecotype - male_ecotype, mate_spec_eco);
}


kewe::attractiveness kewe::calc_attractiveness(
    const individual& female,
    const individual& male,
    const simulation_parameters& p
    )
{
  return calc_attractiveness(
    female.get_fem_pref(),
    male.get_male_trait(),
    p.get_gauss_mate_spec_mate(),
    female.get_eco_trait(),
    male.get_eco_trait(),
    p.get_gauss_mate_spec_eco()
  );
}

kewe::attractiveness kewe::calc_attractiveness(
  const individual& female,
  const individual& male,
  const ribi::gausser& gauss_mate_spec_mate,
  const ribi::gausser& gauss_mate_spec_eco
)
{
  return calc_attractiveness(
    female.get_fem_pref(),
    male.get_male_trait(),
    gauss_mate_spec_mate,
    female.get_eco_trait(),
    male.get_eco_trait(),
    gauss_mate_spec_eco
  );
}

