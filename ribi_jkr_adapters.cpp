#include "ribi_jkr_adapters.h"

#include "ribi_parameters.h"
#include "ribi_results.h"
#include "pbd_ltt.h"
#include <fstream>
#include <iostream>

ribi::simulation ribi::create_simulation(const parameters& p) noexcept
{
  return simulation(p);
}

std::string ribi::get_ltt_plot_filename(const parameters& p) noexcept
{
  return p.get_ltt_plot_filename();
}

int ribi::get_n_generations(const parameters& p) noexcept
{
  return p.get_n_generations();
}


ribi::results ribi::get_results(const simulation& s) noexcept
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
  f << r.get_ltt();
}
