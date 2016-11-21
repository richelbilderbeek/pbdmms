#include "ribi_jkr_wrapper_functions.h"

#include "ribi_parameters.h"
#include "ribi_results.h"

#include <fstream>

ribi::simulation ribi::create_simulation(const parameters& p)
{
  return simulation(p);
}

std::string ribi::get_nltt_plot_filename(const parameters& p)
{
  return p.get_filename_genotype_frequency_graph();
}

ribi::results ribi::get_results(const simulation& s)
{
  return s.get_results();
}

void ribi::run(simulation& s)
{
  s.run();
}

void ribi::save_nll_plot(const results& r, const std::string& filename)
{
  std::ofstream f(filename);
  f << r.get_sil_frequency_phylogeny();
}
