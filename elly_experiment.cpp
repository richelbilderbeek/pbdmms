#include "elly_experiment.h"

#include "elly_simulation.h"
#include "daic_run.h"

elly::experiment::experiment(const parameters& p)
  : m_daisie_output{}, m_simulation_results{}
{
  simulation s(p);
  s.run();
  m_simulation_results = get_results(s);

  //daic::input daisie_input =

  //daic::run()


}
