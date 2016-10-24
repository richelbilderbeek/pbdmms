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

int divide_test()
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

// Create tests for simulation
int jobo_simulation_test()
{
  int n_fails{0};
  //Setting and getting parameters should symmetrical
  {
    const parameters p(42,123,38,0.5);
    const simulation s(p);
    if (s.get_parameters() != p) ++n_fails;
  }

  //A starting simulation should have the right population size
  {
    const parameters p(42,123,38,0.5);
    const simulation s(p);
    if (
      static_cast<int>(s.get_individuals().size())
      != p.get_population_size()
    ) ++n_fails;
  }

  //A starting population has individuals all of the same genotype
  {
    const parameters p(42,123,38,0.5);
    const simulation s(p);
    const auto population = s.get_individuals();
    assert(population.size() >= 2);
    if (population.front() != population.back()) ++n_fails;
  }

  //An individual has the right number of loci
  //test if number of loci is the same as genotype size?
  #ifdef ISSUE_11
  {
    const parameters p(6,123,38,0.5);
    const simulation s(p);
    const auto population = s.get_individuals();
    //assert(population.empty());
    //assert(population.size() >= 1);
    const auto individual = population.front();
    if (p.get_n_loci() != 6)++n_fails;
    if (individual.get_n_loci() != 6)++n_fails;
    if (individual.get_n_loci() != p.get_n_loci()) ++n_fails;
  }
  #endif //ISSUE_11

  //After some time, a new genotype will arise
  {
  }

  //After some time, there will be more species
  {
  }

// Create tests for output
  return n_fails;
}

int main() {
  int n_fails{0};
  try
  {
    hello_jobo();
    if (add(40,2) != 42) ++n_fails;
    n_fails += divide_test();
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
