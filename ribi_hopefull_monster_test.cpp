#include "ribi_hopefull_monster.h"
#include <fstream>
#include <iostream>
#include "ribi_helper.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace ribi;

BOOST_AUTO_TEST_CASE(ribi_hopefull_monster_constructor)
{
  const int generation{10};
  const individual kid(individual::pin_t("AAA"), individual::sil_t(3, 0b010));
  const individual dad(individual::pin_t("AAA"), individual::sil_t(3, 0b010));
  const individual mom(individual::pin_t("AAA"), individual::sil_t(3, 0b010));
  const std::pair<individual,individual> parents(dad, mom);
  BOOST_CHECK_NO_THROW(
    hopefull_monster(generation, kid, parents)
  );
}

#pragma GCC diagnostic pop

