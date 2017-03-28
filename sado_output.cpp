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
#include "sado_mate_graph.h"

void sado::append_histogram(const histogram &p, const std::string& filename)
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

std::vector<sado::species> sado::group_individuals_to_species(
  const population& pop,
  const parameters& p,
  const int gen
)
{
  /// No indivs in this population? return.
  if (pop.empty()) return {};

  assert(all_have_unique_ids(pop.get_population()));


  ///One indiv in population, return 1 species
  const mate_graph g = create_mate_graph(pop, p);

  //c: 'color': the index each component gets assigned, starts from zero
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
  const std::map<int, std::vector<individual>> individuals{
    seperate_individuals_by_id(
      c, g
    )
  };
  std::vector<species> v;
  v.reserve(individuals.size());
  std::transform(
    std::begin(individuals),
    std::end(individuals),
    std::back_inserter(v),
    [gen](const auto& q) { return species(gen, q.second); }
  );
  return v;
}

sado::result sado::measure( //!OCLINT indeed the classic code is too long
  const population& pop,
  const int t,
  const parameters& p)
{
  const int pop_size{static_cast<int>(pop.size())};
  assert(all_have_unique_ids(pop.get_population()));

  const double sp{calc_cssd_p(pop)};
  const double sq{calc_cssd_q(pop)};
  const double sx{calc_cssd_x(pop)};

  const double rhoxp{calc_rhoxp(pop)};
  const double rhoxq{calc_rhoxq(pop)};
  const double rhopq{calc_rhopq(pop)};

  const histogram histp{rescale_max_to_one(create_histogram_p(pop, p))};
  const histogram histq{rescale_max_to_one(create_histogram_q(pop, p))};
  const histogram histx{rescale_max_to_one(create_histogram_x(pop, p))};

  #define OUTPUT_EVERYWHERE
  #ifdef OUTPUT_EVERYWHERE
  std::cout
    << t << ' ' << pop_size << ' ' << rhoxp << ' ' << rhoxq << ' ' << rhopq << '\n'
    << get_mean_x(pop) << ' ' << get_mean_p(pop) << ' ' << get_mean_q(pop) << ' ' << sx << ' ' << sp << ' ' << sq << '\n';

  append_histogram(histx, "eco_traits.csv");
  append_histogram(histp, "fem_prefs.csv");
  append_histogram(histq, "male_traits.csv");
  std::stringstream s;
  {
    const result this_result(
      histp,
      histq,
      histx,
      group_individuals_to_species(pop, p, t),
      pop_size,
      rhopq,
      rhoxp,
      rhoxq,
      std_devs(sp, sq, sx),
      t
    );
    s << this_result;
    // Append to file
    std::ofstream out(p.get_output_filename(), std::ios_base::app);
    out << this_result << '\n';
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
    catch (std::exception &) {}  //!OCLINT keep this catch empty, it means we are beyond the golden output
  }
  #endif // OUTPUT_EVERYWHERE

  return result(
    histp,
    histq,
    histx,
    group_individuals_to_species(pop, p, t),
    pop_size,
    rhopq,
    rhoxp,
    rhoxq,
    std_devs(sp, sq, sx),
    t
  );
}

