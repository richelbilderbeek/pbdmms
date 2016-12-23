#include "kewe_jkr_adapters.h"

#include "kewe_ses.h"

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

void kewe::set_population(kewe::simulation& s, const std::vector<kewe::individual>& next_pop)
{
  s.set_pop(next_pop);
  kewe::parameters p = s.get_parameters();
  s.add_generation_number();
  int t = s.get_generation_number();
  std::vector<std::pair<int,int>> ltt_plot = s.get_ltt_plot();
  if (t%p.m_output_parameters.outputfreq == 0)
    {
      kewe::result_variables data = s.get_result_variables();
      kewe::results results = s.get_results();
      do_measurements(
            t,
            results.m_ecological_trait,
            results.m_female_preference,
            results.m_male_trait,
            p,
            next_pop,
            data,
            ltt_plot
            );

      s.set_result_variables(data);
      s.set_results(results);
      s.set_ltt_plot(ltt_plot);
    }

}
