#include "sado_output.h"
#include "count_undirected_graph_connected_components.h"
#include "sado_helper.h"
#include "sado_parameters.h"
#include "sado_results.h"
#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/range/algorithm/min_element.hpp>
#include "sado_simulation.h"
#include "sado_attractiveness_matrix.h"
#include "sado_individual_graph.h"

void sado::append_histogram(const histogram &p, const std::string &filename)
{
  assert(!p.empty());
  const double m{*std::max_element(std::begin(p), std::end(p))};
  assert(m != 0.0);

  std::stringstream s;
  for (double d : p)
  {
    s << (d / m) << ',';
  }
  std::string t{s.str()};
  assert(!t.empty());
  t.resize(t.size() - 1);

  std::ofstream f(filename, std::ios_base::app);
  f << t << '\n';
}

void sado::copy_indivs_to_species(
  const population& pop,
  const int gen,
  results& r,
  const parameters& p)
{
  /// No indivs in this population? return.
  if (pop.empty()) return;

  ///One indiv in population, return 1 species
  const int n_individuals{static_cast<int>(pop.size())};
  if (n_individuals == 1)
  {
    const species sp(gen, { pop[0] } );
    r.add_species(sp);
    return;
  }
  #define FIX_ISSUE_249
  #ifdef FIX_ISSUE_249
  const individual_graph g = create_genotype_graph(pop, p);

  std::vector<int> c(boost::num_vertices(g));
  const int n_species{
    boost::connected_components(g,
      boost::make_iterator_property_map(
        std::begin(c),
        get(boost::vertex_index, g)
      )
    )
  };
  assert(*boost::range::min_element(c) >= 0);

  ///Copy all individuals to the species number 'c[i]'
  std::vector<species> s(n_species, species(gen));

  const auto vip = boost::vertices(g);
  int i{0};
  for (auto vi = vip.first; vi != vip.second; ++vi, ++i)
  {
    assert(i >= 0);
    assert(i < static_cast<int>(c.size()));
    const int species_index{c[i]};
    assert(species_index >= 0);
    assert(species_index < static_cast<int>(s.size()));
    species& this_species = s[species_index];
    const indiv this_indiv = g[*vi];
    this_species.add_indiv(this_indiv);
  }
  for (auto this_species: s)
  {
    r.add_species(this_species);
  }
  #else
  return; //STUB: do nothing
  #endif
}


void sado::output(
    const population &pop, const int t, const parameters &p, results &r)
{
  const int pop_size{static_cast<int>(pop.size())};
  double ssxx = 0.0, ssxp = 0.0, sspp = 0.0, ssxq = 0.0, ssqq = 0.0, sspq = 0.0;
  const double avgx{get_mean_x(pop)};
  const double avgp{get_mean_p(pop)};
  const double avgq{get_mean_q(pop)};
  for (const auto &i : pop.get_population())
  {
    const double dxi{i.get_x() - avgx};
    const double dpi{i.get_p() - avgp};
    const double dqi{i.get_q() - avgq};
    ssxx += dxi * dxi;
    ssxp += dxi * dpi;
    ssxq += dxi * dqi;
    sspp += dpi * dpi;
    sspq += dpi * dqi;
    ssqq += dqi * dqi;
  }
  const double rhoxp{ssxp / std::sqrt(ssxx * sspp)};
  const double rhoxq{ssxq / std::sqrt(ssxx * ssqq)};
  const double rhopq{sspq / std::sqrt(sspp * ssqq)};
  const double sx{std::sqrt(ssxx / (pop_size - 1.0))};
  const double sp{std::sqrt(sspp / (pop_size - 1.0))};
  const double sq{std::sqrt(ssqq / (pop_size - 1.0))};

  const histogram histp{rescale_max_to_one(create_histogram_p(pop, p))};
  const histogram histq{rescale_max_to_one(create_histogram_q(pop, p))};
  const histogram histx{rescale_max_to_one(create_histogram_x(pop, p))};

  std::stringstream s;
  std::cout << t << ' ' << pop_size << ' ' << rhoxp << ' ' << rhoxq << ' '
            << rhopq << '\n'
            << avgx << ' ' << avgp << ' ' << avgq << ' ' << sx << ' ' << sp
            << ' ' << sq << '\n';

  {
    result this_result;
    this_result.m_histx = histx;
    this_result.m_histp = histp;
    this_result.m_histq = histq;
    this_result.m_rhopq = rhopq;
    this_result.m_rhoxp = rhoxp;
    this_result.m_rhoxq = rhoxq;
    this_result.m_sp = sp;
    this_result.m_sq = sq;
    this_result.m_sx = sx;
    this_result.m_t = t;
    this_result.m_pop_size = pop_size;
    r.add_result(this_result);
    append_histogram(histx, "eco_traits.csv");
    append_histogram(histp, "fem_prefs.csv");
    append_histogram(histq, "male_traits.csv");
    s << this_result;

    // Append to file
    {
      std::ofstream out(p.get_output_filename(), std::ios_base::app);
      out << this_result << '\n';
    }
  }

  if (is_golden_standard(p))
  {
    try
    {
      const std::string golden{get_golden_output().at((t / 10) + 1)};
      const std::string measured{s.str()};
      const histogram golden_values{to_doubles(seperate_string(golden, ','))};
      const histogram measured_values{
          to_doubles(seperate_string(measured, ','))};
      std::clog << "Comparing:\n"
                << "golden  : " << golden << '\n'
                << "measured: " << measured << '\n';
      assert(is_more_or_less_same(golden_values, measured_values));
    }
    catch (std::exception &)
    {
      // OK, is beyond golden output
    }
  }
}
