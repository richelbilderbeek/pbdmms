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

//removes species from island species vector and adds it to both habitats species vector
void island_immigration(std::vector<elly::species> island_species,
                        std::vector<elly::species> both_species,
                        elly::parameters p);

//removes species from both habitats species vector and adds it to mainland species vector
void both_extinction_island(std::vector<elly::species> both_species,
                            std::vector<elly::species> mainland_species,
                            elly::parameters p,
                            std::vector<int> species_in_clade);

//removes species from both habitats species vector and adds it to island species vector
void both_extinction_mainland(std::vector<elly::species> both_species,
                              std::vector<elly::species> island_species,
                              elly::parameters p);

//removes species from both habitats species vector and adds it to mainland species vector,
//also creating a new species in island species vector
void both_anagenesis(std::vector<elly::species> mainland_species,
                     std::vector<elly::species> island_species,
                     std::vector<elly::species> both_species,
                     elly::parameters p,
                     const double time,
                     int& id_counter);

//removes species from both habitats species vector and adds it to mainland species vector,
//also creates two new species in island species vector
void both_cladogenesis_island(std::vector<elly::species> mainland_species,
                              std::vector<elly::species> island_species,
                              std::vector<elly::species> both_species,
                              elly::parameters p,
                              const double time,
                              int& id_counter,
                              std::vector<int> species_in_clade);

//removes species from both habitats species vector and adds it to island species vector,
//also creates two new species in mainland species vector
void both_cladogenesis_mainland(std::vector<elly::species> mainland_species,
                                std::vector<elly::species> island_species,
                                std::vector<elly::species> both_species,
                                elly::parameters p,
                                const double time,
                                int& id_counter);


#endif // ELLY_EVENTFUNCTIONS_H
