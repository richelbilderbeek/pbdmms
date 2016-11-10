#include "pbd_l_table_row.h"
#include <fstream>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include "pbd_l_table.h"
#include "pbd.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace pbd;

BOOST_AUTO_TEST_CASE(pbd_l_table_row_abuse)
{
  const int incipient_level_label{1};
  const int incipient_level_label_parents{42};
  const double t_incipient{1.0};
  const double t_good{1.0};
  const double t_extinction{1.0};
  const int species_level_label{1};

  BOOST_CHECK_NO_THROW(
    l_table_row(
      incipient_level_label,
      incipient_level_label_parents,
      t_incipient,
      t_good,
      t_extinction,
      species_level_label
    )
  );

  BOOST_CHECK_THROW(
    l_table_row(
      0,
      incipient_level_label_parents,
      t_incipient,
      t_good,
      t_extinction,
      species_level_label
    ),
    std::invalid_argument
  );

  //t_incipient can be either positive or -1.0
  BOOST_CHECK_NO_THROW(
    l_table_row(
      incipient_level_label,
      incipient_level_label_parents,
      -1.0,
      t_good,
      t_extinction,
      species_level_label
    )
  );

  BOOST_CHECK_THROW(
    l_table_row(
      incipient_level_label,
      incipient_level_label_parents,
      -0.3,
      t_good,
      t_extinction,
      species_level_label
    ),
    std::invalid_argument
  );

  //t_good can be either positive or -1.0
  BOOST_CHECK_NO_THROW(
    l_table_row(
      incipient_level_label,
      incipient_level_label_parents,
      t_incipient,
      -1.0,
      t_extinction,
      species_level_label
    )
  );

  BOOST_CHECK_THROW(
    l_table_row(
      incipient_level_label,
      incipient_level_label_parents,
      t_incipient,
      -0.2,
      t_extinction,
      species_level_label
    ),
    std::invalid_argument
  );

  //t_extinction can be either positive or -1.0
  BOOST_CHECK_NO_THROW(
    l_table_row(
      incipient_level_label,
      incipient_level_label_parents,
      t_incipient,
      t_good,
      -1.0,
      species_level_label
    )
  );

  BOOST_CHECK_THROW(
    l_table_row(
      incipient_level_label,
      incipient_level_label_parents,
      t_incipient,
      t_good,
      -0.1,
      species_level_label
    ),
    std::invalid_argument
  );

  //species_level_label must be >=1
  BOOST_CHECK_THROW(
    l_table_row(
      incipient_level_label,
      incipient_level_label_parents,
      t_incipient,
      t_good,
      t_extinction,
      0
    ),
    std::invalid_argument
  );
}

#pragma GCC diagnostic pop




