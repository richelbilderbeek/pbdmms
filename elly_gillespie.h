#ifndef ELLY_GILLESPIE_H
#define ELLY_GILLESPIE_H
#include "elly_rates.h"
#include "elly_parameters.h"
#include "elly_species.h"


//draws random waiting time until next event
double draw_waiting_time(const double sumrates, const elly::parameters& p);

//draws random event that takes place
int draw_event(const elly::rates& r , const elly::parameters& p);

//picks species and calls correct eventfunction
void pick_species(int e,
                  std::vector<elly::species> all_species_mainland,
                  std::vector<elly::species> all_species_island,
                  std::vector<elly::species> all_species_both,
                  elly::parameters p,
                  std::vector<elly::species> extinct,
                  int id_counter,
                  std::vector<int> species_in_clades,
                  double time);

#endif // ELLY_GILLESPIE_H
