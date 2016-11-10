#include "pbd_helper.h"
#include <fstream>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include "pbd_l_table.h"
#include "pbd.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace pbd;

BOOST_AUTO_TEST_CASE(pbd_delete_file_use)
{
  const std::string filename{"pbd_delete_file_use.tmp"};
  if (!is_regular_file(filename))
  {
    std::ofstream f(filename);
    f << "something";
  }
  BOOST_CHECK(is_regular_file(filename));
  delete_file(filename);
  BOOST_CHECK(!is_regular_file(filename));
}

BOOST_AUTO_TEST_CASE(pbd_delete_file_abuse)
{
  const std::string filename{"pbd_delete_file_abuse.tmp"};
  if (is_regular_file(filename))
  {
    delete_file(filename);
  }
  BOOST_CHECK(!is_regular_file(filename));
  BOOST_CHECK_THROW(
    delete_file(filename),
    std::invalid_argument
  );
}


BOOST_AUTO_TEST_CASE(pbd_is_regular_file)
{
  const std::string filename{"pbd_is_regular_file.txt"};
  if (is_regular_file(filename)) { delete_file(filename); }
  BOOST_CHECK(!is_regular_file(filename));
  //Create file
  {
    std::fstream f;
    f.open(filename.c_str(),std::ios::out);
    f << "TEMP TEXT";
    f.close();
  }
  BOOST_CHECK(is_regular_file(filename));
  delete_file(filename);
  BOOST_CHECK(!is_regular_file(filename));
}

BOOST_AUTO_TEST_CASE(pbd_remove_first_line_use)
{
  {
    const std::vector<std::string> v{"REMOVE ME", "OK"};
    const std::vector<std::string> w{remove_first(v)};
    BOOST_CHECK_EQUAL(w.size(), 1);
    BOOST_CHECK_EQUAL(w[0], "OK");
  }
}

BOOST_AUTO_TEST_CASE(pbd_remove_first_line_abuse)
{
  BOOST_CHECK_THROW(remove_first({}), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(pbd_seperate_string)
{
  { //Single input, seperator of type char
    const std::vector<std::string> v = seperate_string("a",',');
    BOOST_CHECK(v[0]=="a");
  }
  { //Two inputs, seperator of type char
    const std::vector<std::string> v = seperate_string("a,b",',');
    BOOST_CHECK(v[0]=="a");
    BOOST_CHECK(v[1]=="b");
  }
  { //Five inputs, seperator of type char
    const std::vector<std::string> v = seperate_string("a,bb,ccc,dddd,eeeee",',');
    BOOST_CHECK(v[0]=="a");
    BOOST_CHECK(v[1]=="bb");
    BOOST_CHECK(v[2]=="ccc");
    BOOST_CHECK(v[3]=="dddd");
    BOOST_CHECK(v[4]=="eeeee");
  }
  { //Three inputs, of which one empty, seperator of type char
    const std::vector<std::string> v = seperate_string("a, ,ccc",',');
    BOOST_CHECK(v[0]=="a");
    BOOST_CHECK(v[1]==" ");
    BOOST_CHECK(v[2]=="ccc");
  }
}

#pragma GCC diagnostic pop



