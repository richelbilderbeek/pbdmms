#include "pbd_l_table.h"
#include <fstream>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include "pbd_helper.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace pbd;

BOOST_AUTO_TEST_CASE(pbd_load_l_table_from_csv_zero_rows)
{
  const std::string filename{"pbd_load_l_table_from_csv_zero_rows.csv"};
  {
    std::ofstream f(filename);
    f << "\"\",\"V1\",\"V2\",\"V3\",\"V4\",\"V5\",\"V6\"";
  }
  const auto t = load_l_table_from_csv(filename);
  BOOST_CHECK_EQUAL(t.size(), 0);
  delete_file(filename);
}

BOOST_AUTO_TEST_CASE(pbd_load_l_table_from_csv_one_row)
{
  const std::string filename{"pbd_load_l_table_from_csv_one_row.csv"};
  {
    std::ofstream f(filename);
    f << "\"\",\"V1\",\"V2\",\"V3\",\"V4\",\"V5\",\"V6\"\n";
    f << "\"1\",1,0,0.2000000001,0.2,-1,1";
  }
  const auto t = load_l_table_from_csv(filename);
  BOOST_CHECK_EQUAL(t.size(), 1);
  delete_file(filename);
}

BOOST_AUTO_TEST_CASE(pbd_load_l_table_from_csv_two_rows)
{
  const std::string filename{"pbd_load_l_table_from_csv_two_rows.csv"};
  {
    std::ofstream f(filename);
    f << "\"\",\"V1\",\"V2\",\"V3\",\"V4\",\"V5\",\"V6\"\n";
    f << "\"1\",1,0,0.2000000001,0.2,-1,1\n";
    f << "\"2\",2,1,0.2,0.00198989310153758,-1,2";
  }
  const auto t = load_l_table_from_csv(filename);
  BOOST_CHECK_EQUAL(t.size(), 2);
  delete_file(filename);
}

#pragma GCC diagnostic pop





