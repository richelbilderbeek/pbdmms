#include "kewe_jkr_adapters.h"

#include <fstream>

#include "kewe_ses.h"
#include "kewe_simulation.h"

kewe::simulation kewe::create_simulation(const kewe::parameters& p)
{
  return kewe::simulation(p);
}

std::vector<kewe::individual> kewe::create_next_population(
  const kewe::simulation& s,
  std::mt19937& gen
)
{
  if (static_cast<int>(s.get_pop().size()) < 2)
  {
    throw std::invalid_argument("Population size too small");
  }
  return create_next_generation(s.get_parameters().m_sim_parameters, s.get_pop(), gen);
}

void kewe::run(kewe::simulation& s)
{
  s.run();
}

kewe::results kewe::get_results(const kewe::simulation& s)
{
  return s.get_results();
}

std::string kewe::get_nltt_plot_filename(const parameters& /* p */)
{
  return "kewe_get_nltt_plot_filename.txt";
}

std::string kewe::get_ltt_plot_filename(const kewe::parameters& p)
{
  return p.m_output_parameters.ltt_plot_filename;
}

int kewe::get_n_generations(const kewe::parameters& p)
{
  return p.m_sim_parameters.get_end_time();
}

int kewe::get_rng_seed(const kewe::parameters& p)
{
  return p.m_sim_parameters.seed;
}

void kewe::save_ltt_plot(const kewe::results& r, const std::string& f)
{
  std::ofstream out(f);
  for (int i = 0; i < static_cast<int>(r.m_ltt.size()); ++i)
  {
      assert(i >= 0);
      assert(i < static_cast<int>(r.m_ltt.size()));
       out << r.m_ltt[i].first << ' ' << r.m_ltt[i].second << '\n';
  }
}

void kewe::save_nltt_plot(const results& /* r */, const std::string& /* f */)
{
  assert(!"TODO");
}

void kewe::set_population(kewe::simulation& s, const std::vector<kewe::individual>& next_pop)
{
  s.set_pop(next_pop);
}
