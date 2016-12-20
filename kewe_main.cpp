#include <iostream>
#include <QFile>
#include <string>
#include <fstream>
#include <random>
#include "kewe_simulation.h"
#include "jkr_experiment.h"
#include "kewe_results.h"
#include "kewe_SES.h"
#include "kewe_jkr_adapters.h"

int main()
{
  //====FIX_ISSUE_131====
  QFile f(":/kewe/kewe_testparameters");
  f.copy("testparameters");
  kewe::kewe_parameters parameters = kewe::read_parameters("testparameters");
  kewe::simulation s(parameters);
  s.run();

  jkr::do_experiment<kewe::kewe_parameters, kewe::simulation, kewe::results>(parameters);

  return 0;
}
