#include "elly_events.h"

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
