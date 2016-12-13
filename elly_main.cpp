#include <cassert>
#include <iostream>
#include "elly_parameters.h"
#include "elly_rates.h"
#include "elly_species.h"
#include "elly_gillespie.h"
#include "elly_eventfunctions.h"
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

    double time = 0.0;
    int main_init = 500;
    int id_counter = 0;
    std::vector<elly::species> all_species_mainland;
    std::vector<elly::species> all_species_island;
    std::vector<elly::species> all_species_both;
    std::vector<int> species_in_clade(main_init, 0);
    for(int i = 0; i < main_init; ++i)
    {
      create_species(all_species_mainland, 0, time, id_counter, i);
    }
    //initialising initial mainland species, number of species equal to main_init

    std::vector<elly::species> extinct_species;

    int mo = all_species_mainland.size();
    int io = all_species_island.size();
    int bo = all_species_both.size();
    //setting initial conditions

    std::vector<double> dd_rates_mimm(main_init, 0);
    std::vector<double> dd_rates_iclad(main_init, 0);
    std::vector<double> dd_rates_bcladi(main_init, 0);

    elly::parameters p = create_parameters_set1();


    for(int i = 0; i < 200; ++i){
    elly::rates r = calculate_rates(p, mo, io, bo,
                                          dd_rates_mimm,
                                          dd_rates_iclad,
                                          dd_rates_bcladi,
                                          species_in_clade);
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
                 species_in_clade,
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

