#include "jobo_test_parameters.h"
#include "jobo_parameters.h"
#include <cassert>
#include <exception>
#include <iostream>
#include <fstream>
#include <stdexcept>

int jobo::test_parameters() noexcept
{
  int n_fails{0};

  // Create tests for parameter settings
  const int n_loci{42};
  const int population_size{1000};
  parameters p(n_loci, population_size);
  if (p.get_n_loci() != n_loci) ++n_fails;
  if (p.get_population_size() != population_size) ++n_fails;

  //Cannot have a negative number of loci
  ++n_fails; //This is undone upon success
  try
  {
    parameters p(-1234, 1000);
  }
  catch (std::invalid_argument&)
  {
    --n_fails; //Correct! Undo ++n_fails above
  }

  //Cannot have a negative population size
  ++n_fails; //This is undone upon success
  try
  {
    parameters p(42, -1234);
  }
  catch (std::invalid_argument&)
  {
    --n_fails; //Correct! Undo ++n_fails above
  }

  // test_parameters_copy_and_equality()
    const parameters a(42,1);
    const parameters b(a); //Copy
    const parameters c( 0,1);
    const parameters d(42,0);
    if (a != a) ++n_fails;
    if (a != b) ++n_fails;
    if (a == c) ++n_fails;
    if (a == d) ++n_fails;

    if (b != a) ++n_fails;
    if (b != b) ++n_fails;
    if (b == c) ++n_fails;
    if (b == d) ++n_fails;

    if (c == a) ++n_fails;
    if (c == b) ++n_fails;
    if (c != c) ++n_fails;
    if (c == d) ++n_fails;

    if (d == a) ++n_fails;
    if (d == b) ++n_fails;
    if (d == c) ++n_fails;
    if (d != d) ++n_fails;
    return n_fails;
}



