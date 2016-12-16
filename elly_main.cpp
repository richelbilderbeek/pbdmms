#include <cassert>
#include <iostream>
#include "elly_parameters.h"
#include "elly_rates.h"
#include "elly_species.h"
#include "elly_gillespie.h"
#include "elly_eventfunctions.h"
#include "elly_variables.h"
#include <vector>
#include <random>
#include <chrono>
#include <cstdlib>
#include <exception>

int main()
{
  try
  {
    using namespace elly;


    variables v;

    for(int i = 0; i < v.get_main_init(); ++i)
    {
      create_species(v, 0, time, v.get_id_counter(), i);
    }
    //initialising initial mainland species, number of species equal to main_init

    elly::parameters p = create_parameters_set1();


    for(int i = 0; i < 5; ++i){
    elly::rates r = calculate_rates(p, mo, io, bo, species_in_clades);
    std::cout << r.get_mimm() << '\n';

    time += draw_waiting_time( calc_sumrates(r), p);
    std::cout << time << '\n';

    int e = draw_event(r, p);
    std::cout<< e <<'\n';
    assert(e > 0);
    assert(e < 11);

    pick_species(e,
                 all_species_mainland,
                 all_species_island,
                 all_species_both,
                 p,
                 extinct_species,
                 id_counter,
                 species_in_clades,
                 time);
      }

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
}

