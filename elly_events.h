#ifndef ELLY_EVENTS_H
#define ELLY_EVENTS_H

namespace elly {

enum class event
{
  /// A species becomes a new species
  ana,
  /// A species that is on both location yields two new species. This happens on the island
  clad_glob_on_island,
  /// A species that is on both location yields two new species. This happens on the mainland
  clad_glob_on_main,
  /// A species that is on the island only yields two new species
  clad_island_only,
  /// A species that is on the mainland only yields two new species
  clad_main_only,
  /// A species that is on both location goes extinct on the island only
  ext_glob_on_island,
  /// A species that is on both location goes extinct on the mainland only
  ext_glob_on_main,
  /// A species that lives on the island only goes extinct
  ext_island_only,
  /// A species that lives on the mainland only goes extinct
  ext_main_only,
  /// A species can migrate from mainland to island
  migration_to_island
};

} //~namespace elly

#endif // ELLY_EVENTS_H
