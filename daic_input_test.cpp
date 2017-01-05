#include "daic_input.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

using namespace daic;

BOOST_AUTO_TEST_CASE(daic_create_input_article_has_eight_rows)
{
  const input i = create_input_article();
  BOOST_CHECK_EQUAL(i.get().size(), 8);
}


#pragma GCC diagnostic pop
