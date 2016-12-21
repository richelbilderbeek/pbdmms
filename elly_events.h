#ifndef ELLY_EVENTFUNCTIONS_H
#define ELLY_EVENTFUNCTIONS_H

#include <vector>

#include "elly_fwd.h"

namespace elly {

///adds two new mainland species from the same clade as the parent species,
///also pushes parent species from mainland species vector to extinct species vector
void mainland_cladogenesis(simulation &s);

///pushes random species from mainland species vector to extinct species vector
void mainland_extinction(simulation &s);

///removes species from mainland species vector and adds it to both (mainland and island) vector
void mainland_immigration(simulation &s);

///pushes random species from island species vector to extinct species vector
void island_extinction(simulation &s);

//adds two new island species from the same clade as the parent species,
//also pushes parent species from island species vector to extinct species vector
void island_cladogenesis(simulation &s);

#ifdef ALLOW_COLONIZATION_OF_MAINLAND_FROM_ISLAND
//removes species from island species vector and adds it to both habitats species vector
void island_immigration(simulation &s);
#endif // ALLOW_COLONIZATION_OF_MAINLAND_FROM_ISLAND

//removes species from both habitats species vector and adds it to mainland species vector
void both_extinction_island(simulation &s);

//removes species from both habitats species vector and adds it to island species vector
void both_extinction_mainland(simulation &s);

//removes species from both habitats species vector and adds it to mainland species vector,
//also creating a new species in island species vector
void both_anagenesis(simulation &s);

//removes species from both habitats species vector and adds it to mainland species vector,
//also creates two new species in island species vector
void both_cladogenesis_island(simulation &s);

//removes species from both habitats species vector and adds it to island species vector,
//also creates two new species in mainland species vector
void both_cladogenesis_mainland(simulation &s);


} //~namespace elly

#endif // ELLY_EVENTFUNCTIONS_H
