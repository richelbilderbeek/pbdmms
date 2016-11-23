#include "ribi_jkr_adapters.h"

#include "ribi_parameters.h"
#include "ribi_results.h"
#include "pbd_ltt.h"
#include <fstream>
#include <iostream>

ribi::simulation ribi::create_simulation(const parameters& p)
{
  return simulation(p);
}

std::string ribi::get_ltt_plot_filename(const parameters& p)
{
  return p.get_ltt_plot_filename();
}

ribi::results ribi::get_results(const simulation& s)
{
  return s.get_results();
}

void ribi::run(simulation& s)
{
  s.run();
}

void ribi::save_ltt_plot(const results& r, const std::string& filename)
{
  std::ofstream f(filename);
  //const pbd::ltt my_ltt = r.get_ltt();
  f << r.get_ltt();
}
