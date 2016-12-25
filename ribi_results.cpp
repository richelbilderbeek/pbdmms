#include "ribi_results.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <numeric>
#include <boost/algorithm/string/replace.hpp>
#include "add_bundled_vertex.h"
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"
#include "ribi_sil.h"
#include "ribi_helper.h"
#include "pbd_helper.h"
#include "add_bundled_edge.h"
#include "get_edge_between_vertices.h"
#include "has_edge_between_vertices.h"
#include "is_isomorphic.h"
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"
#include "count_undirected_graph_connected_components.h"
//#include "find_first_bundled_vertex_with_my_vertex.h"


ribi::results::results(
  const int max_genetic_distance
)
  : m_ltt{},
    m_max_genetic_distance{max_genetic_distance},
    m_sil_frequency_phylogeny{},
    m_summarized_sil_frequency_phylogeny{},
    m_vds_prev{}
{
  if (m_max_genetic_distance < 1)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "max_genetic_distance must be >= 1, "
      << "supplied value was " << m_max_genetic_distance
    ;
    throw std::invalid_argument(msg.str());
  }
}

void ribi::results::add_measurement(
  const int t,
  const population& any_population,
  const std::vector<hopefull_monster>& /* hopefull_monsters */
)
{
  if (t < 0)
  {
    std::stringstream msg;
    msg << __func__ << ": generation 't' cannot be "
      << "negative, value supplied is " << t
    ;
    throw std::invalid_argument(msg.str());
  }

  m_ltt.add_timepoint(
    t,
    count_species(any_population, get_max_genetic_distance())
  );
}

/*
void ribi::results::add_measurement(
  const int t,
  const population& any_population,
  const std::vector<hopefull_monster>& hopefull_monsters
)
{
  if (t < 0)
  {
    std::stringstream msg;
    msg << __func__ << ": generation 't' cannot be "
      << "negative, value supplied is " << t
    ;
    throw std::invalid_argument(msg.str());
  }

  m_ltt.add_timepoint(
    t,
    count_species(any_population, get_max_genetic_distance())
  );

  #ifdef FIX_ISSUE_41
  //Add SIL frequencies to graph, collect vertex descriptors
  const sil_frequency_vertex_descriptors vds
    = add_sils(
      any_population, //No need for the full population, a tally of genotypes suffices
      t,
      m_sil_frequency_phylogeny
    )
  ;
  assert(all_vds_have_unique_sil(vds, m_sil_frequency_phylogeny));
  assert(count_sils(vds, m_sil_frequency_phylogeny) == static_cast<int>(any_population.size()));

  //Connect the vertices within this fresh cohort
  connect_species_within_cohort(vds, m_max_genetic_distance, m_sil_frequency_phylogeny);
  assert(count_sils(vds, m_sil_frequency_phylogeny) == static_cast<int>(any_population.size()));
  assert(all_vds_have_same_time(m_vds_prev, m_sil_frequency_phylogeny));
  assert(all_vds_have_unique_sil(vds, m_sil_frequency_phylogeny));

  //Connect the vertices from this fresh cohort to the previous one
  //by connecting the individuals that can mate
  connect_species_between_cohorts(
    vds,
    m_vds_prev,
    m_max_genetic_distance,
    m_sil_frequency_phylogeny
  );

  //Sometimes, individuals are born that cannot mate with their
  //parent. These hopefull monsters are connected to their parents here
  connect_hopefull_monsters(
    hopefull_monsters,
    m_sil_frequency_phylogeny
  );
  assert(count_sils(vds, m_sil_frequency_phylogeny) == static_cast<int>(any_population.size()));
  assert(all_vds_have_unique_sil(vds, m_sil_frequency_phylogeny));
  assert(count_undirected_graph_connected_components(m_sil_frequency_phylogeny) == 1);

  //Keep the newest vds
  m_vds_prev = vds;
  assert(count_sils(m_vds_prev, m_sil_frequency_phylogeny)
    == static_cast<int>(any_population.size())
  );
  #endif // FIX_ISSUE_41
}
*/

std::vector<ribi::sil_frequency_vertex_descriptor> ribi::add_sils(
  const population& latest_population,
  const int t,
  sil_frequency_phylogeny& g
) noexcept
{
  //No need for the full population, a tally of genotypes suffices
  const auto sil_tally = tally_sils(latest_population);
  return add_sils(
    sil_tally,
    t,
    g
  );
}

std::vector<ribi::sil_frequency_vertex_descriptor> ribi::add_sils(
  const std::map<sil,int>& m,
  const int t,
  sil_frequency_phylogeny& g
) noexcept
{
  std::vector<sil_frequency_vertex_descriptor> vds;
  for (const auto p: m)
  {
    std::map<sil,int> sil_frequencies;
    sil_frequencies.insert(std::make_pair(p.first, p.second));

    const auto vd = add_bundled_vertex(
      sil_frequency_vertex(
        sil_frequencies,
        t
      ),
      g
    );
    vds.push_back(vd);
  }
  assert(count_sils(vds, g) == sum_tally(m));
  return vds;
}

bool ribi::all_vds_have_same_id(
  const std::vector<sil_frequency_vertex_descriptor>& vds,
  const sil_frequency_phylogeny& g
) noexcept
{
  if (vds.size() <= 1) return true;
  std::set<int> ts;
  std::transform(std::begin(vds),std::end(vds),
    std::inserter(ts, std::end(ts)),
    [g](const sil_frequency_vertex_descriptor vd)
    {
      return g[vd].get_id();
    }
  );
  return ts.size() == 1;
}

bool ribi::all_vds_have_same_time(
  const std::set<sil_frequency_vertex_descriptor>& vds,
  const sil_frequency_phylogeny& g
) noexcept
{
  if (vds.size() <= 1)
  {
    return true;
  }
  std::set<int> ts;
  std::transform(std::begin(vds),std::end(vds),
    std::inserter(ts, std::end(ts)),
    [g](const sil_frequency_vertex_descriptor vd)
    {
      return g[vd].get_time();
    }
  );
  return ts.size() == 1;
}

bool ribi::all_vds_have_same_time(
  const std::vector<sil_frequency_vertex_descriptor>& vds,
  const sil_frequency_phylogeny& g
) noexcept
{
  if (vds.size() <= 1)
  {
    return true;
  }
  std::set<int> ts;
  std::transform(std::begin(vds),std::end(vds),
    std::inserter(ts, std::end(ts)),
    [g](const sil_frequency_vertex_descriptor vd)
    {
      return g[vd].get_time();
    }
  );
  return ts.size() == 1;
}

bool ribi::all_vds_have_unique_sil(
  const std::vector<sil_frequency_vertex_descriptor>& vds,
  const sil_frequency_phylogeny& g
) noexcept
{
  std::set<sil> sils;
  for (const auto vd: vds)
  {
    for (const auto sf: g[vd].get_sil_frequencies())
    {
      sils.insert(sf.first);
    }
  }
  return sils.size() == vds.size();
}

void ribi::clear_all_sil_frequencies(
  sil_frequency_phylogeny& g
) noexcept
{
  const auto vip = vertices(g);
  std::for_each(vip.first, vip.second,
    [&g](const sil_frequency_vertex_descriptor vd)
    {
      g[vd].clear_sil_frequencies();
    }
  );
}

void ribi::clear_vertex_with_id(
  const int id,
  sil_frequency_phylogeny& g
)
{
  const auto vip = vertices(g);
  const auto vd = std::find_if(
    vip.first, vip.second,
    [g, id](const sil_frequency_vertex_descriptor d) {
      return g[d].get_id() == id;
    }
  );
  if (vd == vip.second)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "could not find vertex with id '"
      << id << "'"
    ;
    throw std::invalid_argument(msg.str());
  }
  boost::clear_vertex(*vd,g);
}

void ribi::connect_hopefull_monster(
  const hopefull_monster& monster,
  sil_frequency_phylogeny& g
)
{
  const sil_frequency_vertex_descriptor vd_kid
    = find_first_with_sil(monster.m_kid.get_sil(), g)
  ;
  const sil_frequency_vertex_descriptor vd_mother
    = find_first_with_sil(monster.m_parents.first.get_sil(), g)
  ;
  const sil_frequency_vertex_descriptor vd_father
    = find_first_with_sil(monster.m_parents.second.get_sil(), g)
  ;

  if (!has_edge_between_vertices(vd_kid, vd_mother, g))
  {
    add_bundled_edge(
      vd_kid,
      vd_mother,
      sil_frequency_edge(1),
      g
    );
  }

  if (!has_edge_between_vertices(vd_kid, vd_father, g))
  {
    add_bundled_edge(
      vd_kid,
      vd_father,
      sil_frequency_edge(1),
      g
    );
  }
}

void ribi::connect_hopefull_monsters(
  const std::vector<hopefull_monster>& hopefull_monsters,
  sil_frequency_phylogeny& g
)
{
  for (const auto monster: hopefull_monsters)
  {
    connect_hopefull_monster(monster, g);
  }
}

void ribi::connect_species_between_cohorts(
  const std::vector<sil_frequency_vertex_descriptor>& vds,
  const std::vector<sil_frequency_vertex_descriptor>& vds_prev,
  const int max_genetic_distance,
  sil_frequency_phylogeny& g
) noexcept
{
  assert(all_vds_have_same_time(vds, g));
  assert(all_vds_have_same_time(vds_prev, g));

  //Connect vertices with the same and close SILs
  const int n_vds{static_cast<int>(vds.size())};
  const int n_vds_prev{static_cast<int>(vds_prev.size())};
  for (int i=0; i!=n_vds; ++i)
  {
    for (int j=0; j!=n_vds_prev; ++j)
    {
      const auto vd_now = vds[i];
      const auto vd_prev = vds_prev[j];
      //const auto t_prev = g[ vd_prev ].get_time();
      //if (t_prev != m_t_prev) continue; //Can only connect to previous cohort
      const auto v_a = g[ vd_now ];
      const auto v_b = g[ vd_prev ];
      const auto silfs_a = v_a.get_sil_frequencies();
      const auto silfs_b = v_b.get_sil_frequencies();
      assert(silfs_a.size() == 1);
      assert(silfs_b.size() == 1);
      const auto sil_a = (*silfs_a.begin()).first;
      const auto sil_b = (*silfs_b.begin()).first;
      assert(sil_a.size() == sil_b.size());
      if (count_different_bits(sil_a, sil_b) <= max_genetic_distance)
      {
        add_bundled_edge(
          vd_now,
          vd_prev,
          sil_frequency_edge(1),
          g
        );
      }
    }
  }

}

void ribi::connect_species_within_cohort(
  const std::vector<sil_frequency_vertex_descriptor>& vds,
  const int max_genetic_distance,
  sil_frequency_phylogeny& g
) noexcept
{
  assert(all_vds_have_same_time(vds, g));

  const int n_vds{static_cast<int>(vds.size())};
  for (int i=0; i!=n_vds; ++i)
  {
    for (int j=i+1; j!=n_vds; ++j)
    {
      const auto v_a = g[ vds[i] ];
      const auto v_b = g[ vds[j] ];
      const auto silfs_a = v_a.get_sil_frequencies();
      const auto silfs_b = v_b.get_sil_frequencies();
      assert(silfs_a.size() == 1);
      assert(silfs_b.size() == 1);
      const auto sil_a = (*silfs_a.begin()).first;
      const auto sil_b = (*silfs_b.begin()).first;
      if (count_different_bits(sil_a, sil_b) <= max_genetic_distance)
      {
        add_bundled_edge(
          vds[i],
          vds[j],
          sil_frequency_edge(0),
          g
        );

      }
    }
  }
}

void ribi::connect_vertices_with_ids(
  const int id_a, const int id_b,
  const sil_frequency_edge& edge,
  sil_frequency_phylogeny& g
)
{
  const auto vip = vertices(g);
  const auto vd_a = std::find_if(
    vip.first, vip.second,
    [g, id_a](const sil_frequency_vertex_descriptor vd) {
      return g[vd].get_id() == id_a;
    }
  );
  assert(vd_a != vip.second);
  const auto vd_b = std::find_if(
    vip.first, vip.second,
    [g, id_b](const sil_frequency_vertex_descriptor vd) {
      return g[vd].get_id() == id_b;
    }
  );
  assert(vd_b != vip.second);
  assert(!has_edge_between_vertices(*vd_a, *vd_b, g));
  add_bundled_edge(*vd_a, *vd_b, edge, g);
}

int ribi::count_sils(
  const std::vector<sil_frequency_vertex_descriptor>& vds,
  const sil_frequency_phylogeny& g
) noexcept
{
  return static_cast<int>(
    std::accumulate(
      std::begin(vds),std::end(vds),
      0,
      [g](const int sum, const sil_frequency_vertex_descriptor vd)
      {
        return sum + sum_tally(g[vd].get_sil_frequencies());
      }
    )
  );
}

ribi::sil_frequency_vertex_descriptor ribi::find_common_ancestor(
  sil_frequency_vertex_descriptors vds,
  const sil_frequency_phylogeny& g
)
{
  assert(vds.size() >= 2);
  assert(all_vds_have_same_time(vds, g));
  assert(!all_vds_have_same_id(vds, g));

  while (!all_vds_have_same_id(vds, g))
  {
    for (auto& vd : vds)
    {
      const auto older = get_older(vd, g);
      assert(!older.empty());
      vd = older.back(); //Just pick one
    }
  }
  assert(!vds.empty());
  return vds.back();
}

ribi::sil_frequency_vertex_descriptor ribi::find_first_with_sil(
  const ribi::sil& s,
  const ribi::sil_frequency_phylogeny& g
)
{
  const auto vip = vertices(g);
  const auto i = std::find_if(
    vip.first, vip.second,
    [s,g](const auto d) { return has_sil(g[d], s); }
  );
  if (i == vip.second)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "could not find SIL '"
      << s << "'"
    ;
    throw std::invalid_argument(msg.str());
  }
  return *i;

}

ribi::sil_frequency_vertex_descriptor_pairs ribi::find_splits_and_mergers(
  sil_frequency_phylogeny& g
) noexcept
{
  sil_frequency_vertex_descriptor_pairs v;
  const auto vip = vertices(g);
  for (auto vi  = vip.first; vi != vip.second; ++vi)
  {
    const auto w = find_splits_and_mergers_from_here(*vi, g);
    std::copy(std::begin(w), std::end(w), std::back_inserter(v));
  }
  return v;
}

ribi::sil_frequency_vertex_descriptor_pairs
ribi::find_splits_and_mergers_from_here(
  const sil_frequency_vertex_descriptor vd,
  sil_frequency_phylogeny& g
) noexcept
{
  sil_frequency_vertex_descriptor_pairs p; //Empty

  const sil_frequency_vertex_descriptors v{
    get_older(vd, g)
  };
  assert(all_vds_have_same_time(v, g));
  if (v.size() < 2) return p;
  const sil_frequency_vertex_descriptor common_ancestor{
    find_common_ancestor(v, g)
  };

  p.push_back(std::make_pair(vd, common_ancestor));
  return p;
}


void ribi::fuse_vertices_with_same_sil_frequencies(
  sil_frequency_phylogeny& g
)
{
  while(1)
  {
    const auto before = g;
    const auto vip = vertices(g);
    for (auto vi = vip.first; vi != vip.second; ++vi)
    {
      fuse_vertices_with_same_sil_frequencies_once_from_here(*vi, g);
    }
    remove_unconnected_empty_vertices(g);
    if (is_isomorphic(before, g)) return;
  }
}

void ribi::fuse_vertices_with_same_sil_frequencies_once_from_here(
  const sil_frequency_vertex_descriptor vd,
  sil_frequency_phylogeny& g
) noexcept
{
  //Collect all neighbours with
  // * the same style
  // * the same genotype frequencies
  // * a later generation
  // * a degree of 2
  const auto& focal_sfs = g[vd].get_sil_frequencies();
  const auto focal_style = g[vd].get_style();
  const auto focal_time = g[vd].get_time();

  //Its neighbour
  const auto neighbors = boost::adjacent_vertices(vd, g);

  for (auto neighbor = neighbors.first; neighbor != neighbors.second; ++neighbor)
  {
    assert(has_edge_between_vertices(vd, *neighbor, g));
    //Only neighbours same genotype frequencies
    if (focal_sfs != g[*neighbor].get_sil_frequencies()) continue;
    //Only neighbours with same style
    if (focal_style != g[*neighbor].get_style()) continue;
    //Only neighbours from different generations
    if (focal_time >= g[*neighbor].get_time()) continue;
    //Only neighbours with two neighbours count
    if (degree(*neighbor, g) != 2) continue;

    fuse_vertices_with_same_sil_frequencies_once_from_here_via_there(vd, *neighbor, g);
  }
}

void ribi::fuse_vertices_with_same_sil_frequencies_once_from_here_via_there(
  const sil_frequency_vertex_descriptor vd,
  const sil_frequency_vertex_descriptor neighbor,
  sil_frequency_phylogeny& g
) noexcept
{
  const auto next_neighbors = boost::adjacent_vertices(neighbor, g);
  for (auto next_neighbor = next_neighbors.first;
    next_neighbor != next_neighbors.second;
    ++next_neighbor
  )
  {
    if(!has_edge_between_vertices(neighbor, *next_neighbor, g))
    {
      continue; //Already removed by someone else
    }
    assert(has_edge_between_vertices(neighbor, *next_neighbor, g));
    //Do not get back the focal vertex
    if (*next_neighbor == vd) continue;
    //Next neighbor may have different SIL frequencies
    //Next neighbor may have different style
    //Only next neighbors from older generations
    if (g[neighbor].get_time() >= g[*next_neighbor].get_time()) continue;

    fuse_vertices_with_same_sil_frequencies(vd, neighbor, *next_neighbor, g);
  }
}

void ribi::fuse_vertices_with_same_sil_frequencies(
  const sil_frequency_vertex_descriptor vd,
  const sil_frequency_vertex_descriptor neighbor,
  const sil_frequency_vertex_descriptor next_neighbor,
  sil_frequency_phylogeny& g
)
{
  assert(all_different(vd, neighbor, next_neighbor));
  //What is the edge length from focal vertex to next neighbour?
  const auto ed_a = get_edge_between_vertices(vd, neighbor, g);
  const auto ed_b = get_edge_between_vertices(neighbor, next_neighbor, g);
  //What is the replacement edge length?
  // vd --- 1 --- neighbour --- 2 --- next_neighbor
  //Becomes
  // vd ------------- 3 ------------- next_neighbor
  const auto l_a = g[ed_a].get_n_timesteps();
  const auto l_b = g[ed_b].get_n_timesteps();
  const auto l_c = l_a + l_b;
  assert(has_edge_between_vertices(vd, neighbor, g));
  assert(has_edge_between_vertices(neighbor, next_neighbor, g));
  assert(!has_edge_between_vertices(vd, next_neighbor, g));
  assert(vd != next_neighbor);
  const int vd_id = g[vd].get_id();
  const int neighbor_id = g[neighbor].get_id();
  const int next_neighbor_id = g[next_neighbor].get_id();
  assert(all_different(vd_id, neighbor_id, next_neighbor_id));
  assert(
    all_different(
      g[vd].get_time(),
      g[neighbor].get_time(),
      g[next_neighbor].get_time()
    )
  );

  //Styles must match between first and second vertex
  assert(g[vd].get_style() == g[neighbor].get_style());
  //SFs must match between first and second vertex
  assert(g[vd].get_sil_frequencies() == g[neighbor].get_sil_frequencies());

  //These to not invalidate vertex descriptors
  g[neighbor].clear_sil_frequencies();
  clear_vertex_with_id(neighbor_id, g);
  connect_vertices_with_ids(
    vd_id, next_neighbor_id,
    sil_frequency_edge(l_c),
    g
  );
}

void ribi::fuse_vertices_with_same_style(
  const sil_frequency_vertex_descriptor vd,
  const sil_frequency_vertex_descriptor neighbor,
  const sil_frequency_vertex_descriptor next_neighbor,
  sil_frequency_phylogeny& g
)
{
  assert(all_different(vd, neighbor, next_neighbor));
  //What is the edge length from focal vertex to next neighbour?
  const auto ed_a = get_edge_between_vertices(vd, neighbor, g);
  const auto ed_b = get_edge_between_vertices(neighbor, next_neighbor, g);
  //What is the replacement edge length?
  // vd --- 1 --- neighbour --- 2 --- next_neighbor
  //Becomes
  // vd ------------- 3 ------------- next_neighbor
  const auto l_a = g[ed_a].get_n_timesteps();
  const auto l_b = g[ed_b].get_n_timesteps();
  const auto l_c = l_a + l_b;
  assert(has_edge_between_vertices(vd, neighbor, g));
  assert(has_edge_between_vertices(neighbor, next_neighbor, g));
  assert(!has_edge_between_vertices(vd, next_neighbor, g));
  assert(vd != next_neighbor);
  const int vd_id = g[vd].get_id();
  const int neighbor_id = g[neighbor].get_id();
  const int next_neighbor_id = g[next_neighbor].get_id();
  assert(all_different(vd_id, neighbor_id, next_neighbor_id));
  //These to not invalidate vertex descriptors
  clear_vertex_with_id(neighbor_id, g);
  connect_vertices_with_ids(
    vd_id, next_neighbor_id,
    sil_frequency_edge(l_c),
    g
  );
}

void ribi::fuse_vertices_with_same_style(
  sil_frequency_phylogeny& g
) noexcept
{
  while(1)
  {
    const auto before = g;
    const auto vip = vertices(g);
    for (auto vi = vip.first; vi != vip.second; ++vi)
    {
      fuse_vertices_with_same_style_once_from_here(*vi, g);
    }
    remove_unconnected_empty_vertices(g);
    if (is_isomorphic(before, g)) return;
  }
}

void ribi::fuse_vertices_with_same_style_once_from_here(
  const sil_frequency_vertex_descriptor vd,
  sil_frequency_phylogeny& g
) noexcept
{
  //Collect all neighbours with
  // * the same style
  // * a degree of 2
  const auto focal_style = g[vd].get_style();

  //Its neighbour
  const auto neighbors = boost::adjacent_vertices(vd, g);

  for (auto neighbor = neighbors.first; neighbor != neighbors.second; ++neighbor)
  {
    assert(has_edge_between_vertices(vd, *neighbor, g));
    //Only neighbours same style
    if (focal_style != g[*neighbor].get_style()) continue;
    //Only neighbours with two neighbours count
    if (degree(*neighbor, g) != 2) continue;

    fuse_vertices_with_same_style_once_from_here_via_there(vd, *neighbor, g);
  }
}

void ribi::fuse_vertices_with_same_style_once_from_here_via_there(
  const sil_frequency_vertex_descriptor vd,
  const sil_frequency_vertex_descriptor neighbor,
  sil_frequency_phylogeny& g
) noexcept
{
  const auto focal_style = g[vd].get_style();

  const auto next_neighbors = boost::adjacent_vertices(neighbor, g);
  for (auto next_neighbor = next_neighbors.first;
    next_neighbor != next_neighbors.second;
    ++next_neighbor
  )
  {
    if(!has_edge_between_vertices(neighbor, *next_neighbor, g))
    {
      continue; //Already removed by someone else
    }
    assert(has_edge_between_vertices(neighbor, *next_neighbor, g));
    //Do not get back the focal vertex
    if (*next_neighbor == vd) continue;
    //Only next neighbours with same style
    if (focal_style != g[*next_neighbor].get_style()) continue;

    fuse_vertices_with_same_style(vd, neighbor, *next_neighbor, g);
  }

}

std::string ribi::get_filename_bs_dot(const std::string& user_filename) noexcept
{
  const std::string base_filename = boost::replace_last_copy(user_filename, ".dot", "");
  const std::string base_filename_bs = base_filename + "_bs";
  return base_filename_bs + ".dot";
}

std::string ribi::get_filename_bs_png(const std::string& user_filename) noexcept
{
  const std::string base_filename = boost::replace_last_copy(user_filename, ".dot", "");
  const std::string base_filename_bs = base_filename + "_bs";
  return base_filename_bs + ".png";
}

std::string ribi::get_filename_bs_svg(const std::string& user_filename) noexcept
{
  const std::string base_filename = boost::replace_last_copy(user_filename, ".dot", "");
  const std::string base_filename_bs = base_filename + "_bs";
  return base_filename_bs + ".svg";
}

std::string ribi::get_filename_dot(const std::string& user_filename) noexcept
{
  const std::string base_filename = boost::replace_last_copy(user_filename, ".dot", "");
  return base_filename + ".dot";
}

std::string ribi::get_filename_png(const std::string& user_filename) noexcept
{
  const std::string base_filename = boost::replace_last_copy(user_filename, ".dot", "");
  return base_filename + ".png";
}

std::string ribi::get_filename_svg(const std::string& user_filename) noexcept
{
  const std::string base_filename = boost::replace_last_copy(user_filename, ".dot", "");
  return base_filename + ".svg";
}

ribi::sil_frequency_phylogeny ribi::results::get_sil_frequency_phylogeny() const noexcept
{
  return m_sil_frequency_phylogeny;
}

///Get the time of the oldest vertex
int ribi::get_time_oldest(
  const sil_frequency_vertex_descriptors& vds,
  const sil_frequency_phylogeny& g
)
{
  const auto there = std::max_element(
    std::begin(vds),
    std::end(vds),
    [g](const auto lhs, const auto rhs)
    {
      return g[lhs].get_time() < g[rhs].get_time();
    }
  );
  return g[*there].get_time();
}

ribi::sil_frequency_vertex_descriptors ribi::get_older(
  sil_frequency_vertex_descriptor vd,
  const sil_frequency_phylogeny& g
)
{
  ribi::sil_frequency_vertex_descriptors v;
  const auto aip = boost::adjacent_vertices(vd, g);
  for (auto ai = aip.first; ai != aip.second; ++ai)
  {
    if (g[*ai].get_time() < g[vd].get_time())
    {
      v.push_back(*ai);
    }
  }
  //Not all vds may have the same time yet:
  /*
      1   1
    +---B---C
    |
    A
    |   2
    +-------D

  In this case, v will consist of B and D.
  With get_older_of_same_time this is achieved

  */
  get_older_of_same_time(v, g);
  assert(all_vds_have_same_time(v, g));
  return v;
}

void ribi::get_older_of_same_time(
  sil_frequency_vertex_descriptors& vds,
  const sil_frequency_phylogeny& g
)
{
  if (all_vds_have_same_time(vds, g)) return;

  //Not all vds may have the same time yet:
  /*
      1   1
    +---B---C
    |
    A
    |   2
    +-------D

  In this case, v will consist of B and D.
  With get_older_of_same_time this is achieved

  */
  const auto t_oldest = get_time_oldest(vds, g);
  for (auto& vd: vds)
  {
    while (g[vd].get_time() != t_oldest)
    {
      assert(g[vd].get_time() < t_oldest);
      const auto vds_older = get_older(vd, g);
      assert(!vds_older.empty());
      vd = vds_older.back(); //Use last of many, unsure if this is wise
    }
  }


}

ribi::sil_frequency_vertex_descriptors ribi::get_older(
  sil_frequency_vertex_descriptors vds,
  const sil_frequency_phylogeny& g
)
{
  std::set<sil_frequency_vertex_descriptor> older;
  for (const auto vd: vds)
  {
    const auto v = get_older(vd, g);
    std::copy(std::begin(v), std::end(v), std::inserter(older, std::end(older)));
  }
  assert(all_vds_have_same_time(older, g));
  sil_frequency_vertex_descriptors result;
  std::copy(std::begin(older), std::end(older), std::back_inserter(result));
  assert(all_vds_have_same_time(result, g));
  return result;
}

ribi::sil_frequency_vertex_descriptors ribi::get_younger(
  sil_frequency_vertex_descriptor vd,
  const sil_frequency_phylogeny& g
)
{
  ribi::sil_frequency_vertex_descriptors v;
  const auto aip = boost::adjacent_vertices(vd, g);
  for (auto ai = aip.first; ai != aip.second; ++ai)
  {
    if (g[*ai].get_time() > g[vd].get_time())
    {
      v.push_back(*ai);
    }
  }
  return v;
}

ribi::sil_frequency_vertex_descriptors ribi::get_younger(
  sil_frequency_vertex_descriptors vds,
  const sil_frequency_phylogeny& g
)
{
  std::set<sil_frequency_vertex_descriptor> younger;
  for (const auto vd: vds)
  {
    const auto v = get_younger(vd, g);
    std::copy(std::begin(v), std::end(v), std::inserter(younger, std::end(younger)));
  }
  sil_frequency_vertex_descriptors result;
  std::copy(std::begin(younger), std::end(younger), std::back_inserter(result));
  return result;
}

void ribi::remove_unconnected_empty_vertices(
  sil_frequency_phylogeny& g
) noexcept
{
  const auto vds = vertices(g);
  for (int i = boost::num_vertices(g) - 1; i!=-1; --i)
  {
    const auto vd = vds.first + i;
    if (g[*vd].get_sil_frequencies().empty() && degree(*vd, g) == 0)
    {
      boost::remove_vertex(*vd, g);
    }
  }
}

void ribi::remove_vertex_with_id(
  const int id,
  sil_frequency_phylogeny& g
)
{
  const auto vip = vertices(g);
  const auto vd = std::find_if(
    vip.first, vip.second,
    [g, id](const sil_frequency_vertex_descriptor d) {
      return g[d].get_id() == id;
    }
  );
  if (vd == vip.second)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "could not find vertex with id '"
      << id << "'"
    ;
    throw std::invalid_argument(msg.str());
  }
  boost::clear_vertex(*vd,g);
  boost::remove_vertex(*vd,g);
}

void ribi::set_all_vertices_styles(
  sil_frequency_phylogeny& g,
  const int max_genetic_distance) noexcept
{
  const auto vip = vertices(g);
  const auto j = vip.second;
  for (auto i=vip.first; i!=j; ++i)
  {
    const int n_possible_species{
      count_n_possible_species(g[*i], max_genetic_distance)
    };
    assert(n_possible_species != 0);
    const sil_frequency_vertex_style style{
      n_possible_species == 1
      ? sil_frequency_vertex_style::good
      : sil_frequency_vertex_style::incipient
    };
    g[*i].set_style(style);
  }
}

void ribi::results::summarize_sil_frequency_phylogeny()
{
  //All individuals must be connected
  assert(count_undirected_graph_connected_components(m_sil_frequency_phylogeny) == 1);

  m_summarized_sil_frequency_phylogeny = m_sil_frequency_phylogeny;

  //Merge the genotypes at the same point in time
  summarize_genotypes(
    m_summarized_sil_frequency_phylogeny
  );

  assert(count_undirected_graph_connected_components(m_summarized_sil_frequency_phylogeny) == 1);

  set_all_vertices_styles(
    m_summarized_sil_frequency_phylogeny,
    m_max_genetic_distance
  );

  assert(count_undirected_graph_connected_components(m_summarized_sil_frequency_phylogeny) == 1);

  clear_all_sil_frequencies(
    m_summarized_sil_frequency_phylogeny
  );

  assert(count_undirected_graph_connected_components(m_summarized_sil_frequency_phylogeny) == 1);

  fuse_vertices_with_same_style(
    m_summarized_sil_frequency_phylogeny
  );

  assert(count_undirected_graph_connected_components(m_summarized_sil_frequency_phylogeny) == 1);

  zip(m_summarized_sil_frequency_phylogeny);

  assert(count_undirected_graph_connected_components(m_summarized_sil_frequency_phylogeny) == 1);

  fuse_vertices_with_same_sil_frequencies(
    m_summarized_sil_frequency_phylogeny
  );

  assert(count_undirected_graph_connected_components(m_summarized_sil_frequency_phylogeny) == 1);
}


/*
void ribi::results::summarize_sil_frequency_phylogeny()
{
  //All individuals must be connected
  assert(count_undirected_graph_connected_components(m_sil_frequency_phylogeny) == 1);

  m_summarized_sil_frequency_phylogeny = m_sil_frequency_phylogeny;

  for (int i=0; i!=8; ++i)
  {
    {
      std::stringstream s; s << i << ".dot";
      if (pbd::is_regular_file(s.str())) pbd::delete_file(s.str());
    }
    {
      std::stringstream s; s << i << ".svg";
      if (pbd::is_regular_file(s.str())) pbd::delete_file(s.str());
    }
    {
      std::stringstream s; s << i << ".png";
      if (pbd::is_regular_file(s.str())) pbd::delete_file(s.str());
    }
  }

  save_to_png(m_summarized_sil_frequency_phylogeny, "1");

  //Merge the genotypes at the same point in time
  summarize_genotypes(
    m_summarized_sil_frequency_phylogeny
  );

  save_to_png(m_summarized_sil_frequency_phylogeny, "2");
  assert(count_undirected_graph_connected_components(m_summarized_sil_frequency_phylogeny) == 1);

  set_all_vertices_styles(
    m_summarized_sil_frequency_phylogeny,
    m_max_genetic_distance
  );

  save_to_png(m_summarized_sil_frequency_phylogeny, "3");
  assert(count_undirected_graph_connected_components(m_summarized_sil_frequency_phylogeny) == 1);

  clear_all_sil_frequencies(
    m_summarized_sil_frequency_phylogeny
  );

  save_to_png(m_summarized_sil_frequency_phylogeny, "4");
  assert(count_undirected_graph_connected_components(m_summarized_sil_frequency_phylogeny) == 1);

  fuse_vertices_with_same_style(
    m_summarized_sil_frequency_phylogeny
  );

  save_to_png(m_summarized_sil_frequency_phylogeny, "5");
  assert(count_undirected_graph_connected_components(m_summarized_sil_frequency_phylogeny) == 1);

  zip(m_summarized_sil_frequency_phylogeny);

  save_to_png(m_summarized_sil_frequency_phylogeny, "6");
  assert(count_undirected_graph_connected_components(m_summarized_sil_frequency_phylogeny) == 1);

  fuse_vertices_with_same_sil_frequencies(
    m_summarized_sil_frequency_phylogeny
  );

  save_to_png(m_summarized_sil_frequency_phylogeny, "7");
  assert(count_undirected_graph_connected_components(m_summarized_sil_frequency_phylogeny) == 1);
}
*/

void ribi::results::save(const std::string& user_filename) const
{
  {
    std::ofstream f(get_filename_dot(user_filename));
    f << get_summarized_sil_frequency_phylogeny();
  }
  convert_dot_to_svg(get_filename_dot(user_filename), get_filename_svg(user_filename));
  convert_svg_to_png(get_filename_svg(user_filename), get_filename_png(user_filename));
}

void ribi::results::save_all(const std::string& user_filename)
{
  //Save before summary
  {
    std::ofstream f(get_filename_bs_dot(user_filename));
    f << get_sil_frequency_phylogeny();
  }
  convert_dot_to_svg(get_filename_bs_dot(user_filename), get_filename_bs_svg(user_filename));
  convert_svg_to_png(get_filename_bs_svg(user_filename), get_filename_bs_png(user_filename));

  summarize_sil_frequency_phylogeny(); //Must summarize
  save(user_filename);
}

void ribi::summarize_genotypes(sil_frequency_phylogeny& g)
{
  /*

  A --- B ---- C -> A     B --- C
  |                       |
  |                       |
  D                       D

  Where timepoints of A, B and D are equal

  */
  const auto vds = vertices(g);
  for (auto vi = vds.first; vi != vds.second; ++vi)
  {
    summarize_genotypes_from_here(*vi, g);
  }
  remove_unconnected_empty_vertices(g);

  if (count_undirected_graph_connected_components(g) != 1)
  {
    std::cerr << "ERROR: count_undirected_graph_connected_components(g) == "
      << count_undirected_graph_connected_components(g) << '\n'
    ;
  }
}

void ribi::summarize_genotypes_from_here(
  const sil_frequency_vertex_descriptor vd,
  sil_frequency_phylogeny& g
)
{
  //Collect all neighbours of vd
  std::vector<sil_frequency_vertex_descriptor> nvds; //Neighbor Vertex Descriptors
  const auto nip = boost::adjacent_vertices(vd, g); //Neighbor Iterator Pair
  if (nip.first == nip.second)
  {
    //No neighbors
    return;
  }
  for (auto ni = nip.first; ni != nip.second; ++ni)
  {
    nvds.push_back(*ni);
  }
  assert(!nvds.empty());
  //Partition these in neighbours of same and different generation
  const auto t = g[vd].get_time();
  std::partition(std::begin(nvds), std::end(nvds),
    [g, t](const auto nvd) { return t == g[nvd].get_time(); }
  );
  //First neighbor is from different generation? Then nothing can be done
  if (g[nvds.front()].get_time() != t)
  {
    //No neighbors from same generation
    return;
  }
  //Move SIL frequencies to first neighbor
  move_sil_frequencies(g[vd], g[nvds.front()]);  //From, to

  //Transfer connections to first neighbor
  for (auto onvd = std::begin(nvds) + 1; onvd != std::end(nvds); ++onvd)
  {
    if (!has_edge_between_vertices(nvds.front(), *onvd, g))
    {
      add_bundled_edge(
        nvds.front(),
        *onvd,
        sil_frequency_edge(1),
        g
      );
    }
  }

  //Clear the current connections
  boost::clear_vertex(vd, g);
}



void ribi::zip(
  sil_frequency_phylogeny& g
) noexcept
{
  while (1)
  {
    const sil_frequency_vertex_descriptor_pairs v = find_splits_and_mergers(g);

    //No vertices? Done
    if (v.empty()) return;

    /*
       --A
          \
           X--present  ->  --A==X--present
          /
       --B                   B
    */
    zip(v, g);
    remove_unconnected_empty_vertices(g);
  }
}

void ribi::zip(
  const sil_frequency_vertex_descriptor_pairs& v,
  sil_frequency_phylogeny& g
) noexcept
{
  for (const auto p: v)
  {
    zip(p, g);
  }
}

void ribi::zip(
  const sil_frequency_vertex_descriptor_pair& split_and_merger,
  sil_frequency_phylogeny& g
) noexcept
{
  //Define a primary line (e.g. via 2)
  //and one or more secondary lines (in this case via 3)
  /*

           2
          / \
 Past 0--1   4--5 Present
          \ /
           3

  */
  auto first_older = get_older(split_and_merger.first, g);
  assert(first_older.size() >= 2);
  //Define the primary line
  sil_frequency_vertex_descriptor vd_primary = first_older.back();

  //Let the secondary lines follow the other older vertices
  first_older.pop_back();
  sil_frequency_vertex_descriptors vds_secondary = first_older;

  while (vd_primary != split_and_merger.second)
  {
    //Get older primary, must be done before edge is deleted by move_sil_connections
    const auto older_primaries = get_older(vd_primary, g);
    assert(older_primaries.size() == 1);
    const auto older_secondaries = get_older(vds_secondary, g);

    //Move content to primary
    move_sil_frequencies(vds_secondary, vd_primary, g);

    //Move connections to primary
    move_sil_connections(vds_secondary, vd_primary, g);

    //Move to older
    vd_primary = older_primaries.back();
    vds_secondary = older_secondaries;
  }

}
