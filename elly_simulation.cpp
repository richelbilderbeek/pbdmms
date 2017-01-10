#include "elly_simulation.h"
#include "elly_populations.h"

#include <cassert>
#include <iostream>
#include <cstdlib>
#include <exception>
#include <vector>

#include "elly_location.h"
#include "elly_clade_id.h"
#include "elly_species.h"
#include "elly_species_id.h"
#include "elly_event_rates.h"
#include "elly_gillespie.h"
#include "elly_clade.h"

elly::simulation::simulation(
  const parameters& p
)
  : m_measurements{},
    m_parameters{p},
    m_populations(p),
    m_rng(p.get_rng_seed()),
    m_t{0.0}
{

}

elly::species elly::simulation::get_random_species(location where, std::mt19937& rng)const noexcept
{
  return m_populations.get_random_species(where, rng);
}

int elly::simulation::count_species(const location where) const noexcept
{
  return m_populations.count_species(where);
}

int elly::simulation::count_species(const clade_id& id) const noexcept
{
  return m_populations.count_species(id);
}

void elly::simulation::do_next_event()
{
  const event_rates r(m_parameters, *this);

  const double t_to_event{draw_waiting_time(r, m_rng)};
  assert(t_to_event > 0.0);
  const event e{draw_event(r, get_rng())};
  do_next_event(t_to_event, e);
}

void elly::simulation::do_next_event(const double t_to_event, const event e)
{
  //Do the measurements
  //At this moment, these are the rates
  const event_rates r(m_parameters, *this);
  m_measurements.push_back(measurement(m_t, m_populations, r));

  m_t += t_to_event;
  do_event(e, *this);
}

void elly::simulation::do_next_event(
  const double t_to_event, const event e, const species& s)
{
  //Do the measurements
  //At this moment, these are the rates
  const event_rates r(m_parameters, *this);
  m_measurements.push_back(measurement(m_t, m_populations, r));

  m_t += t_to_event;
  do_event(e, s, *this);
}

elly::species elly::simulation::extract_random_species(const location any_location)
{
  return m_populations.extract_random_species(any_location, m_rng);
}

elly::simulation elly::replay_for_input_article_light() noexcept //!OCLINT Must be this long for now
{
  //Create parameters to supply 3 species for a crown age of 10 mya
  const double rate_clado_main{0.2}; //Irrelevant
  const double rate_clado_is{0.2}; //Irrelevant
  const double rate_ana{0.87}; //Irrelevant
  const double rate_ext_is{0.1}; //Irrelevant
  const double rate_ext_main{0.1}; //Irrelevant
  const double rate_mig_to_is{0.02}; //Irrelevant
  const int carryingcap_is{30}; //Irrelevant
  const int carryingcap_main{10}; //Irrelevant
  const unsigned int rng_seed{385}; //Irrelevant
  const int init_n_mainland{3}; //The number of clades
  const double crown_age{10.0}; //Must be after all event
  const parameters p(
    rate_clado_main,
    rate_clado_is,
    rate_ana,
    rate_ext_is,
    rate_ext_main,
    rate_mig_to_is,
    carryingcap_is,
    carryingcap_main,
    rng_seed,
    init_n_mainland,
    crown_age
  );
  //+-----------+--------------------+----+---------------------------+
  //| cn        | status             | ms | Branching_times           |
  //+-----------+--------------------+----+---------------------------+
  //| Coccyzus  | Non_endemic_MaxAge |  0 | 7.456                     |
  //| Dendroica | Non_endemic        |  0 | 0.34                      |
  //| Finches   | Endemic            |  0 | 3.0282,1.3227,0.8223      |
  //+-----------+--------------------+----+---------------------------+
  /* Coccyzus:

  |
  |   ON MAINLAND:
  |
  |   +---+---+---+---+ c
  |   |
  +-d-+   +---@---X     a (Coccyzus)
  |   |   |
  |   +-e-+
  |       |
  |       +---+---+---+ b
  |
  |   ON ISLAND:
  |
  |           @---+---+ a (Coccyzus)
  |
  |   OVERESTIMATION:
  |
  |       #---+---+---+ a (Coccyzus)
  |
  +---+---+---+---+---+
  0  t1  t2  t3  t4  t5 timepoint (million years), not to scale

 * At t=0,
   * species d is born at mainland
 * At t=tc1, cladogenesis_mainland_only
   * d gives rise to c at mainland
   * d gives rise to e at mainland
   * d goes extinct
 * At t=tc2=7.456, cladogenesis_mainland_only
   * e gives rise to a at mainland
   * e gives rise to b at mainland
   * e goes extinct
 * At t=tc3, colonization
   * a colonizes the island, a is thus a global species
     the migration is marked with @
 * At t=tc4, extinct_mainland
   * a goes extict on the mainland, a becomes an island-only species
     the extinction is marked with X
  */
  const double tc1{7.456 - 1.0};
  const double tc2{7.456};
  const double tc3{7.456 + 1.0};
  const double tc4{7.456 + 2.0};

  simulation s(p);
  assert(s.get_populations().get_species().size() == 3);
  const species coccyzus_d = s.get_populations().get_species()[0];
  //const species dendroica = s.get_populations().get_species()[1];
  //const species finches = s.get_populations().get_species()[2];
  s.do_next_event(tc1, event::clad_main_only, coccyzus_d);
  assert(collect_kids(coccyzus_d, s.get_populations().get_species()).size() == 2);
  const species coccyzus_c{collect_kids(coccyzus_d, s.get_populations().get_species())[0]};
  const species coccyzus_e{collect_kids(coccyzus_d, s.get_populations().get_species())[1]};
  assert(coccyzus_c != coccyzus_e);
  s.do_next_event(tc2 - tc1, event::clad_main_only, coccyzus_e);
  assert(collect_kids(coccyzus_e, s.get_populations().get_species()).size() == 2);
  const species_id coccyzus_a{
    collect_kids(
      coccyzus_e, s.get_populations().get_species()
    )[0].get_species_id()
  };
  const species_id coccyzus_b{
    collect_kids(
      coccyzus_e, s.get_populations().get_species()
    )[1].get_species_id()
  };
  assert(coccyzus_a != coccyzus_b);

  s.do_next_event(tc3 - tc2, event::migration_to_island, get_species_with_id(coccyzus_a, s));
  s.do_next_event(tc4 - tc3, event::ext_glob_on_main, get_species_with_id(coccyzus_a, s));
  return s;
}

elly::simulation elly::replay_for_input_article() noexcept
{
  //STUB
  return replay_for_input_article_light();
}

void elly::simulation::run()
{
  //Initial populations are already initialized
  assert(m_parameters.get_init_n_mainland()
    == m_populations.count_species(location::mainland_only)
  );

  const double t_end{m_parameters.get_crown_age()};
  while (m_t < t_end)
  {
    do_next_event();
  }
}

void elly::cladogenesis_mainland_only(simulation& s)
{
  cladogenesis_mainland_only(s.get_populations(), s.get_time(), s.get_rng());
}
void elly::cladogenesis_mainland_only(simulation& sim, const species& s)
{
  cladogenesis_mainland_only(sim.get_populations(), sim.get_time(), s);
}

elly::species elly::get_species_with_id(
  const species_id id,
  const simulation& s
)
{
  return get_species_with_id(id, s.get_populations());
}

void elly::mainland_extinction(simulation& s)
{
  mainland_extinction(s.get_populations(), s.get_time(), s.get_rng());
}
void elly::mainland_extinction(simulation& sim, const species& s)
{
  mainland_extinction(sim.get_populations(), sim.get_time(), s);
}

void elly::mainland_immigration(simulation& s)
{
  mainland_immigration(s.get_populations(), s.get_time(), s.get_rng());
}
void elly::mainland_immigration(simulation& sim, const species& s)
{
  mainland_immigration(sim.get_populations(), sim.get_time(), s);
}

void elly::island_extinction(simulation& s)
{
  island_extinction(s.get_populations(), s.get_time(), s.get_rng());
}
void elly::island_extinction(simulation& sim, const species& s)
{
  island_extinction(sim.get_populations(), sim.get_time(), s);
}

void elly::cladogenesis_island_only(simulation& s)
{
  assert(s.count_species(location::island_only) > 0);
  cladogenesis_island_only(s.get_populations(), s.get_time(), s.get_rng());
}
void elly::cladogenesis_island_only(simulation& sim, const species& s)
{
  assert(sim.count_species(location::island_only) > 0);
  cladogenesis_island_only(sim.get_populations(), sim.get_time(), s);
}

void elly::both_extinction_island(simulation &s)
{
  both_extinction_island(s.get_populations(), s.get_time(), s.get_rng());
}
void elly::both_extinction_island(simulation &sim, const species& s)
{
  both_extinction_island(sim.get_populations(), sim.get_time(), s);
}

void elly::both_extinction_mainland(simulation& s)
{
  both_extinction_mainland(s.get_populations(), s.get_time(), s.get_rng());
}
void elly::both_extinction_mainland(simulation& sim, const species& s)
{
  both_extinction_mainland(sim.get_populations(), sim.get_time(), s);
}

void elly::both_anagenesis(simulation &s)
{
  both_anagenesis(s.get_populations(), s.get_time(), s.get_rng());
}
void elly::both_anagenesis(simulation &sim, const species& s)
{
  both_anagenesis(sim.get_populations(), sim.get_time(), s);
}

void elly::cladogenesis_global_on_island(simulation &s)
{
  cladogenesis_global_on_island(s.get_populations(), s.get_time(), s.get_rng());
}
void elly::cladogenesis_global_on_island(simulation &sim, const species& s)
{
  cladogenesis_global_on_island(sim.get_populations(), sim.get_time(), s);
}

void elly::cladogenesis_global_on_mainland(simulation &s)
{
  cladogenesis_global_on_mainland(s.get_populations(), s.get_time(), s.get_rng());
}
void elly::cladogenesis_global_on_mainland(simulation &sim, const species& s)
{
  cladogenesis_global_on_mainland(sim.get_populations(), sim.get_time(), s);
}
