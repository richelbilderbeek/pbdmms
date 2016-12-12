#ifndef ELLY_GILLESPIE_H
#define ELLY_GILLESPIE_H
#include "elly_rates.h"
#include "elly_parameters.h"


//draws random waiting time until next event
double draw_waiting_time(const double sumrates, const elly::parameters& p);

//draws random event that takes place
int draw_event(const elly::rates& r , const elly::parameters& p);

#endif // ELLY_GILLESPIE_H
