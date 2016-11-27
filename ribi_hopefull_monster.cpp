#include "ribi_hopefull_monster.h"

#include <cassert>

ribi::hopefull_monster::hopefull_monster(
  const int generation,
  const individual& kid,
  const std::pair<individual,individual>& parents
)
  : m_generation{generation},
    m_kid{kid},
    m_parents{parents}
{
  assert(generation >= 0);
}
