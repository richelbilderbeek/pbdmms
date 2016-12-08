#ifndef ELLY_EVENTFUNCTIONS_H
#define ELLY_EVENTFUNCTIONS_H
#include "elly_species.h"
#include "elly_parameters.h"

void mainland_cladogenesis(std::vector<elly::species> mainland_species,
                           std::vector<elly::species> extinct_species,
                           elly::parameters p);
void mainland_extinction();
void mainland_immigration();
void island_extinction();
void island_cladogenesis();
void island_immigration();
void both_extinction_island();
void both_extinction_mainland();
void both_anagenesis();
void both_cladogenesis_island();
void both_cladogenesis_mainland();


#endif // ELLY_EVENTFUNCTIONS_H
