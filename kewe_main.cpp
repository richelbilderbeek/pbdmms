#include <iostream>
#include <QFile>
#include <string>
#include <fstream>
#include "kewe_simulation.h"
#include "jkr_experiment.h"
#include "kewe_results.h"

simulation create_simulation(const kewe_parameters& p)
{
  return simulation(p);
}

void run(simulation& s)
{
  s.run();
}

results get_results(const simulation& s)
{
  return s.get_results();
}

std::string get_ltt_plot_filename(const kewe_parameters& p)
{
  return p.output_parameters.ltt_plot_filename;
}

void save_ltt_plot(const results& r, const std::string& f)
{
  std::ofstream out(f);
  for (int i = 0; i < static_cast<int>(r.m_ltt.size()); ++i)
  {
       out << r.m_ltt[i].first << ' ' << r.m_ltt[i].second << '\n';
  }
}

int main()
{
  QFile f(":/kewe/kewe_testparameters");
  f.copy("testparameters");
  kewe_parameters parameters = read_parameters("testparameters");
  simulation s(parameters);
  s.run();



  jkr::do_experiment<kewe_parameters, simulation, results>(parameters);

  return 0;
}
