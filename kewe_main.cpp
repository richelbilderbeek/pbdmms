#include <iostream>
#include <QFile>
#include "kewe_simulation.h"

int main()
{
  QFile f(":/kewe/kewe_testparameters");
  f.copy("testparameters");
  kewe_parameters parameters = read_parameters("testparameters");
  simulation s(parameters);
  s.run();

  return 0;
}
