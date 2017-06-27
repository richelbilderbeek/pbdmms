#ifndef JKR_EXPERIMENT_H
#define JKR_EXPERIMENT_H

#include <random>
//No headers here

namespace jkr {

///Do the experiment
///This is just a first setup, that needs to be extended,
///but let's do this one step at a time
///Note that I use free functions to allow a template wrapper, instead
///of enforcing class member function names.
template <class parameters, class simulation, class results, class ancestry_graph>
void do_experiment(const parameters& p)
{
  simulation s = create_simulation(p);
  const int n_generations = get_n_generations(p);
  std::mt19937 rng_engine(get_rng_seed(p));
  for (int t=0; t!=n_generations; ++t)
  {
    const auto next_population
      = create_next_population(
        const_cast<const simulation&>(s), //Enforce only reading the simulation
        rng_engine
      );
    set_population(s, next_population);
  }
  save_ltt_plot(get_results(s), get_ltt_plot_filename(p));
  save_nltt_plot(get_results(s), get_nltt_plot_filename(p));

  const auto g = create_ancestry_graph(get_results(s));
  const auto h = create_reconstructed(g);
  save_reconstructed_tree(h, get_reconstructed_tree_filename(p));
}

} //~namespace jkr

#endif // JKR_EXPERIMENT_H
