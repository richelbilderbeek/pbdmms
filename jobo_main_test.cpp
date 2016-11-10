//#include "distancer_simulation.h"
#include <cassert>
#include <exception>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "jobo_helper.h"
#include "jobo_parameters.h"
#include "jobo_parameters_test.h"
#include "jobo_simulation.h"
#include "jobo_output.h"
#include "jobo_individual.h"
#include "jobo_individual_test.h"
#include "jobo_individuals.h"
#include "jobo_simulation_test.h"
#include <vector>

using namespace jobo;

//Seperate small program tests
int divide_test()
{
int n_fails{0};

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

int count_good_species_test()
{
int n_fails{0};
std::vector<int>chances{2,2,2,2,2,2,2,2,2,0,2,2,2,2,2};
const int n_good_species = (count_good_species(chances));
//try
if (n_good_species !=5) ++n_fails;

return n_fails;
}

int main() {
  int n_fails{0};

  try
  {
    hello_jobo();
    if (add(40,2) != 42) ++n_fails;
    n_fails += divide_test();
    n_fails += count_good_species_test();
    n_fails += parameters_test();
    n_fails += simulation_test();
    n_fails += individual_test();
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
