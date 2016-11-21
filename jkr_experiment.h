#ifndef JKR_EXPERIMENT_H
#define JKR_EXPERIMENT_H

//No headers here

namespace jkr {

///Do the experiment
///This is just a first setup, that needs to be extended,
///but let's do this one step at a time
///Note that I use free functions to allow a template wrapper, instead
///of enforcing class member function names.
template <class parameters, class simulation, class results>
void do_experiment(const parameters& p)
{
  simulation s = create_simulation(p);
  run(s);
  save_nll_plot(get_results(s), get_nltt_plot_filename(p));
}

} //~namespace jkr

#endif // JKR_EXPERIMENT_H
