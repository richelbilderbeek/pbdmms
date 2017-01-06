#include "elly_events.h"

#include <cassert>

std::string elly::to_str(const event e) noexcept
{
  switch (e)
  {
    case event::ana: return "ana";
    case event::clad_glob_on_island: return "clad_glob_on_island";
    case event::clad_glob_on_main: return "clad_glob_on_main";
    case event::clad_island_only: return "clad_island_only";
    case event::clad_main_only: return "clad_main_only";
    case event::ext_glob_on_island: return "ext_glob_on_island";
    case event::ext_glob_on_main: return "ext_glob_on_main";
    case event::ext_island_only: return "ext_island_only";
    case event::ext_main_only: return "ext_main_only";
    case event::migration_to_island: return "migration_to_island";
  }
  assert(!"Should not get here");
}

std::vector<elly::event> elly::collect_all_events() noexcept
{
  return {
    event::ana,
    event::clad_glob_on_island,
    event::clad_glob_on_main,
    event::clad_island_only,
    event::clad_main_only,
    event::ext_glob_on_island,
    event::ext_glob_on_main,
    event::ext_island_only,
    event::ext_main_only,
    event::migration_to_island
  };
}
