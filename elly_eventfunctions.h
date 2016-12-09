#ifndef ELLY_EVENTFUNCTIONS_H
#define ELLY_EVENTFUNCTIONS_H
#include "elly_species.h"
#include "elly_parameters.h"

//adds two new mainland species from the same clade as the parent species,
//also pushes parent species from mainland species vector to extinct species vector
void mainland_cladogenesis(std::vector<elly::species> mainland_species,
                           std::vector<elly::species> extinct_species,
                           elly::parameters p,
                           int id_counter,
                           const double time);

//pushes random species from mainland species vector to extinct species vector
void mainland_extinction(std::vector<elly::species> mainland_species,
                         std::vector<elly::species> extinct_species,
                         elly::parameters p,
                         const double time);

//removes species from mainland species vector and adds it to both (mainland and island) vector
void mainland_immigration(std::vector<elly::species> mainland_species,
                          std::vector<elly::species> both_species,
                          elly::parameters p,
                          std::vector<int> species_in_clade);

//pushes random species from island species vector to extinct species vector
void island_extinction(std::vector<elly::species> island_species,
                       std::vector<elly::species> extinct_species,
                       elly::parameters p,
                       const double time,
                       std::vector<int> clade_counter);

//adds two new island species from the same clade as the parent species,
//also pushes parent species from island species vector to extinct species vector
void island_cladogenesis(std::vector<elly::species> island_species,
                         std::vector<elly::species> extinct_species,
                         elly::parameters p,
                         int id_counter,
                         const double time,
                         std::vector<int> clade_counter);

void island_immigration();
void both_extinction_island();
void both_extinction_mainland();
void both_anagenesis();
void both_cladogenesis_island();
void both_cladogenesis_mainland();


#endif // ELLY_EVENTFUNCTIONS_H
