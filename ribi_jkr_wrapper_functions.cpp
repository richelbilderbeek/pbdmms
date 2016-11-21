#include "ribi_jkr_wrapper_functions.h"

#include "ribi_parameters.h"
#include "ribi_results.h"

#include <fstream>

ribi::simulation ribi::create_simulation(const parameters& p)
{
  return simulation(p);
}

void ribi::run(const simulation& s)
{
  s.run();
}

void ribi::save_nll_plot(const results& r, const std::string& filename)
{
  std::ofstream f(filename);
  f << r.get_sil_frequency_phylogeny();
}

void save_phylogeny(const results& r, const std::string& filename)
{
  std::ofstream f(filename);
  f << r.
}
