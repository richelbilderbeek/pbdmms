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
    for(int i = 0; i < main_init; ++i)
    {
      create_species(all_species_mainland, 0, time, id_counter);
    }

    std::vector<elly::species> extinct_species;

    int mo = all_species_mainland.size();
    int io = all_species_island.size();
    int bo = all_species_both.size();
    //setting initial conditions

    elly::parameters p = create_parameters_set1();

    const elly::rates r = calculate_rates(p, mo, io, bo );
    std::cout << r.get_mclad() << '\n';

    time += draw_waiting_time( r, p);
    std::cout << time << '\n';

    int e = draw_event(r, p);
    std::cout<< e <<'\n';
    assert(e > 0);
    assert(e < 11);

    switch(e){
      case 0: //mainland cladogenesis
        break;
      case 1: //mainland extinction
        break;
      case 2: //mainland immigration
        break;
      case 3: //island extinction
        break;
      case 4: //island cladogenesis
        break;
      case 5: //island immigration
        break;
      case 6: //both habitats mainland extinction
        break;
      case 7: //both habitats island extinction
        break;
      case 8: //both habitats anagenesis
        break;
      case 9: //both habitats island cladogenesis
        break;
      case 10: //both habitats mainland cladogenesis
        break;
      default:
        throw std::logic_error("drawn event that does not exist");
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

