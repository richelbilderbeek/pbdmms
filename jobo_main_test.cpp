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


// Create tests for parameter settings
int test_parameters()
{
  int n_fails{0};

  //Is the number of loci correctly set and get?
  const int n_loci{42};
  jobo_parameters p(n_loci);
  if (p.get_n_loci() != n_loci) ++n_fails;


  //Cannot have a negative number of loci
  ++n_fails; //This is undone upon success
  try
  {
    jobo_parameters p(-1234);
  }
  catch (std::invalid_argument&)
  {
    --n_fails; //Correct! Undo ++n_fails above
  }

  return n_fails;
}

/*{
  int n_fails = 0;
  if (number_of_species != 1) ++n_fails;

  return n_fails;
}
*/

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

int main() {
  int n_fails{0};
  try
  {
    hello_jobo();
    if (add(40,2) != 42) ++n_fails;
    n_fails += test_divide();
    n_fails += test_parameters();
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
