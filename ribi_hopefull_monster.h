#ifndef RIBI_HOPEFULL_MONSTER_H
#define RIBI_HOPEFULL_MONSTER_H

#include "ribi_individual.h"

namespace ribi {

///A hopefull monster is a kid
///cannot mate with its parents
class hopefull_monster
{
public:
  /// @param generation generation the kid is born
  hopefull_monster(
    const int generation,
    const individual& kid,
    const std::pair<individual,individual>& parents
  );

  const int m_generation;
  const individual m_kid;
  const std::pair<individual,individual> m_parents;
};

} //~namespace ribi

#endif // RIBI_HOPEFULL_MONSTER_H
