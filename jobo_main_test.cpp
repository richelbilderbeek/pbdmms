//#include "distancer_simulation.h"
#include <cassert>
#include <exception>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "jobo_helper.h"
#include "jobo_parameters.h"
#include "jobo_simulation.h"
#include "jobo_output.h"
#include "jobo_individual.h"
#include "jobo_individuals.h"

//
int test_parameters_copy_and_equality()
{
  int n_fails{0};
  const jobo_parameters a(42, -1234);
  const jobo_parameters b(a); //Copy
  const jobo_parameters c( 0, -1234);
  const jobo_parameters d(42,     0);
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

// Create tests for parameter settings
int test_parameters()
{
  int n_fails{0};

  n_fails += test_parameters_copy_and_equality();

  //Is the number of loci correctly set and get?
  const int n_loci{42};
  const int population_size{1000};
  jobo_parameters p(n_loci, population_size);
  if (p.get_n_loci() != n_loci) ++n_fails;
  if (p.get_population_size() != population_size) ++n_fails;


  //Cannot have a negative number of loci
  ++n_fails; //This is undone upon success
  try
  {
    jobo_parameters p(-1234, 1000);
  }
  catch (std::invalid_argument&)
  {
    --n_fails; //Correct! Undo ++n_fails above
  }  

  //Cannot have a negative population size
  ++n_fails; //This is undone upon success
  try
  {
    jobo_parameters p(42, -1234);
  }
  catch (std::invalid_argument&)
  {
    --n_fails; //Correct! Undo ++n_fails above
  }

  return n_fails;
}



// Create tests for simulation
// Create tests for output

int test_divide()
{
  int n_fails = 0;

  //Use
  if (divide(1,2) != 0.5) ++n_fails;

  //Abuse
  try
  {
    divide(2,0);
    ++n_fails; //Should have thrown!
  }
  catch(std::exception&)
  {
    //OK
  }
  return n_fails;
}

int test_jobo_simulation()
{
  int n_fails{0};

  const jobo_parameters p(42,1000);
  const jobo_simulation s(p);
  if (s.get_parameters() != p) ++n_fails;

  if (s.get_individuals().size() != p.get_population_size()) ++n_fails;

  return n_fails;
}


int main() {
  int n_fails{0};
  try
  {
    hello_jobo();
    if (add(40,2) != 42) ++n_fails;
    n_fails += test_divide();
    n_fails += test_parameters();
    n_fails += test_jobo_simulation();
  }

  catch (std::exception& e)
  {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
  catch (...)
  {
    std::cerr << "Error: Unknown\n";
    return 1;
  }
  if (n_fails)
  {
    std::cerr << "FAIL: " << n_fails << " fails\n";
  }
  return n_fails;
}
