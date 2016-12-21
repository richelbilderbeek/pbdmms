#ifndef ELLY_LOCATION_H
#define ELLY_LOCATION_H

namespace elly {

///Places where a species can be
/// * mainland_only: only on mainland
/// * mainland: mainland, includes those species that are present at both locations
/// * island_only: only on island
/// * island: island, includes those species that are present at both locations
/// * both: both locations
enum class location
{
  mainland,
  mainland_only,
  island,
  island_only,
  both
};

} //~namespace elly

#endif // ELLY_LOCATION_H
