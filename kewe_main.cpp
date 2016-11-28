#include <iostream>
#include <QFile>
#include <string>
#include <fstream>
#include <random>
#include "kewe_simulation.h"
#include "jkr_experiment.h"
#include "kewe_results.h"
#include "kewe_SES.h"

simulation create_simulation(const kewe_parameters& p)
{
  return simulation(p);
}

std::vector<indiv> create_next_population(
        const simulation& s,
        std::mt19937&
      )
{
  if (static_cast<int>(s.get_pop().size()) < 2)
    throw std::invalid_argument("Population size too small");
  return create_next_generation(s.get_parameters(),s.get_pop());
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

int  get_n_generations(const kewe_parameters& p)
{
  return p.sim_parameters.endtime;
}

int get_rng_seed(const kewe_parameters& p)
{
  return p.sim_parameters.seed;
}

void save_ltt_plot(const results& r, const std::string& f)
{
  std::ofstream out(f);
  for (int i = 0; i < static_cast<int>(r.m_ltt.size()); ++i)
  {
       out << r.m_ltt[i].first << ' ' << r.m_ltt[i].second << '\n';
  }
}

void set_population(simulation& s, const std::vector<indiv>& next_pop)
{
  s.set_pop(next_pop);
  s.add_generation_number();
}

int main()
{
  QFile f(":/kewe/kewe_testparameters");
  f.copy("testparameters");
  kewe_parameters parameters = read_parameters("testparameters");
  //simulation s(parameters);
  //s.run();

  jkr::do_experiment<kewe_parameters, simulation, results>(parameters);

  return 0;
}
