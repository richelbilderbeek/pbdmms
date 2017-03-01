#include "ribi_jkr_adapters.h"

#include "ribi_parameters.h"
#include "ribi_results.h"
#include "pbd_ltt.h"
#include "pbd_nltt.h"
#include <fstream>
#include <iostream>

void ribi::save_nltt_plot(const results& r, const std::string& filename)
{
  std::ofstream f(filename);
  f << convert_to_nltt(r.get_ltt());
}

void ribi::save_ltt_plot(const results& r, const std::string& filename)
{
  std::ofstream f(filename);
  f << r.get_ltt();
}
