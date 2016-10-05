#include "distancer_results.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <boost/algorithm/string/replace.hpp>
#include "add_bundled_vertex.h"
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"
#include "distancer_sil.h"
#include "distancer_helper.h"
#include "add_bundled_edge.h"
#include "get_edge_between_vertices.h"
#include "has_edge_between_vertices.h"

results::results(
  const int max_genetic_distance
)
  : m_max_genetic_distance{max_genetic_distance},
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

void results::add_measurement(
  const int t,
  const population& any_population
) noexcept
{
  if (t < 0)
  {
    std::stringstream msg;
    msg << __func__ << ": generation 't' cannot be "
      << "negative, value supplied is " << t
    ;
    throw std::invalid_argument(msg.str());
  }

  //Tally SILs
  const std::map<sil,int> m = tally_sils(any_population);

  //Add SIL frequencies to graph, collect vertex descriptors
  const std::vector<sil_frequency_vertex_descriptor> vds = add_sils(m, t, m_sil_frequency_phylogeny);
  assert(all_vds_have_unique_sil(vds, m_sil_frequency_phylogeny));
  assert(count_sils(vds, m_sil_frequency_phylogeny) == static_cast<int>(any_population.size()));

  //Connect the vertices within this fresh cohort
  connect_species_within_cohort(vds, m_max_genetic_distance, m_sil_frequency_phylogeny);
  assert(count_sils(vds, m_sil_frequency_phylogeny) == static_cast<int>(any_population.size()));
  assert(all_vds_have_same_time(m_vds_prev, m_sil_frequency_phylogeny));
  assert(all_vds_have_unique_sil(vds, m_sil_frequency_phylogeny));

  //Connect the vertices from this fresh cohort to the previus one
  connect_species_between_cohorts(
    vds,
    m_vds_prev,
    m_max_genetic_distance,
    m_sil_frequency_phylogeny
  );
  assert(count_sils(vds, m_sil_frequency_phylogeny) == static_cast<int>(any_population.size()));
  assert(all_vds_have_unique_sil(vds, m_sil_frequency_phylogeny));

  //Keep the newest vds
  m_vds_prev = vds;
  assert(count_sils(m_vds_prev, m_sil_frequency_phylogeny) == static_cast<int>(any_population.size()));
}

std::vector<sil_frequency_vertex_descriptor> add_sils(
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

bool all_vds_have_same_time(
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
      return g[vd].get_time();
    }
  );
  return ts.size() == 1;


}

bool all_vds_have_unique_sil(
  const std::vector<sil_frequency_vertex_descriptor>& vds,
  const sil_frequency_phylogeny& g
) noexcept
{
  std::set<sil> sils;
  std::transform(std::begin(vds),std::end(vds),
    std::inserter(sils, std::end(sils)),
    [g](const sil_frequency_vertex_descriptor vd)
    {
      const auto sfs = g[vd].get_sil_frequencies();
      assert(sfs.size() == 1);
      return (*std::begin(sfs)).first;
    }
  );
  return sils.size() == vds.size();
}

void clear_all_sil_frequencies(
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

void connect_species_between_cohorts(
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
      assert(i >= 0);
      assert(i < n_vds);
      const auto vd_now = vds[i];
      assert(j >= 0);
      assert(j < n_vds_prev);
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

void connect_species_within_cohort(
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
      assert(i >= 0);
      assert(i < n_vds);
      assert(j >= 0);
      assert(j < n_vds);
      assert(i != j);
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

void connect_vertices_with_ids(
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
  if (vd_a == vip.second)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "could not find vertex with id '"
      << id_a << "'"
    ;
    throw std::invalid_argument(msg.str());
  }
  const auto vd_b = std::find_if(
    vip.first, vip.second,
    [g, id_b](const sil_frequency_vertex_descriptor vd) {
      return g[vd].get_id() == id_b;
    }
  );
  if (vd_b == vip.second)
  {
    std::stringstream msg;
    msg << __func__ << ": "
      << "could not find vertex with id '"
      << id_b << "'"
    ;
    throw std::invalid_argument(msg.str());
  }
  assert(!has_edge_between_vertices(*vd_a, *vd_b, g));
  add_bundled_edge(*vd_a, *vd_b, edge, g);
}

int count_sils(
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

void fuse_vertices_with_same_style(
  sil_frequency_phylogeny& g
) noexcept
{
  if (boost::num_vertices(g) == 0) return;

  start_from_scratch: ;

  const auto vds = vertices(g);
  //Focal vertex
  for (auto vd = vds.first; vd != vds.second; ++vd)
  {
    const auto focal_style = g[*vd].get_style();

    //look_for_fresh_neighbors: ;

    //Its neighbour
    const auto neighbors = boost::adjacent_vertices(*vd, g);

    for (auto neighbor = neighbors.first; neighbor != neighbors.second; ++neighbor)
    {
      assert(has_edge_between_vertices(*vd, *neighbor, g));
      //Only neighbours same style
      if (focal_style != g[*neighbor].get_style()) continue;
      //Only neighbours with two neighbours count
      if (degree(*neighbor, g) != 2) continue;
      const auto next_neighbors = boost::adjacent_vertices(*neighbor, g);
      for (auto next_neighbor = next_neighbors.first; next_neighbor != next_neighbors.second; ++next_neighbor)
      {
        assert(has_edge_between_vertices(*neighbor, *next_neighbor, g));
        //Do not get back the focal vertex
        if (*next_neighbor == *vd) continue;
        //Only next neighbours with same style
        if (focal_style != g[*next_neighbor].get_style()) continue;
        assert(*vd != *neighbor);
        assert(*vd != *next_neighbor);
        assert(*neighbor != *next_neighbor);
        //What is the edge length from focal vertix to next neighbour?
        const auto ed_a = get_edge_between_vertices(*vd, *neighbor, g);
        const auto ed_b = get_edge_between_vertices(*neighbor, *next_neighbor, g);
        const auto l_a = g[ed_a].get_n_timesteps();
        const auto l_b = g[ed_b].get_n_timesteps();
        const auto l_c = l_a + l_b;
        assert(has_edge_between_vertices(*vd, *neighbor, g));
        assert(has_edge_between_vertices(*neighbor, *next_neighbor, g));
        assert(!has_edge_between_vertices(*vd, *next_neighbor, g));
        assert(*vd != *next_neighbor);
        const int vd_id = g[*vd].get_id();
        const int neighbor_id = g[*neighbor].get_id();
        const int next_neighbor_id = g[*next_neighbor].get_id();
        assert(vd_id != neighbor_id);
        assert(vd_id != next_neighbor_id);
        assert(neighbor_id != next_neighbor_id);
        //These to not invalidate vertex descriptors
        remove_vertex_with_id(neighbor_id, g);
        connect_vertices_with_ids(
          vd_id, next_neighbor_id,
          sil_frequency_edge(l_c),
          g
        );
        goto start_from_scratch;
      }
    }
  }
  remove_unconnected_empty_vertices(g);
}


void remove_unconnected_empty_vertices(
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

void remove_vertex_with_id(
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

void set_all_vertices_styles(
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

void results::summarize_sil_frequency_phylogeny()
{
  m_summarized_sil_frequency_phylogeny = summarize_genotypes(m_sil_frequency_phylogeny);
  set_all_vertices_styles(
    m_summarized_sil_frequency_phylogeny,
    m_max_genetic_distance
  );
  clear_all_sil_frequencies(
    m_summarized_sil_frequency_phylogeny
  );
  fuse_vertices_with_same_style(
    m_summarized_sil_frequency_phylogeny
  );

}


void results::save_all(const std::string& user_filename)
{
  const std::string base_filename = boost::replace_last_copy(user_filename, ".dot", "");
  const std::string filename_dot = base_filename + ".dot";
  const std::string filename_png = base_filename + ".png";
  const std::string filename_svg = base_filename + ".svg";
  // bs: before summary
  const std::string base_filename_bs = base_filename + "_bs";
  const std::string filename_bs_dot = base_filename_bs + ".dot";
  const std::string filename_bs_png = base_filename_bs + ".png";
  const std::string filename_bs_svg = base_filename_bs + ".svg";

  //Save before summary
  {
    std::ofstream f(filename_bs_dot);
    f << get_sil_frequency_phylogeny();
  }
  convert_dot_to_svg(filename_bs_dot, filename_bs_svg);
  convert_svg_to_png(filename_bs_svg, filename_bs_png);

  summarize_sil_frequency_phylogeny(); //Must summarize

  {
    std::ofstream f(filename_dot);
    f << get_summarized_sil_frequency_phylogeny();
  }
  convert_dot_to_svg(filename_dot, filename_svg);
  convert_svg_to_png(filename_svg, filename_png);

}

sil_frequency_phylogeny summarize_genotypes(sil_frequency_phylogeny g)
{
  if (boost::num_vertices(g) == 0) return g;

  const auto vds = vertices(g);
  for (auto vd = vds.first; vd != vds.second; ++vd)
  {
    const auto t = g[*vd].get_time();
    //For all vertices, find the neighbors
    const auto neighbors = boost::adjacent_vertices(*vd, g);
    for (auto neighbor = neighbors.first; neighbor != neighbors.second; ++neighbor)
    {
      //If a neighbor is of the same generation, move all connections and genotypes to it
      const auto t_neighbor = g[*neighbor].get_time();
      if (t != t_neighbor) continue; //Nope
      //Move genotypes
      try
      {
        assert(*vd != *neighbor);
        move_sil_frequencies(g[*vd], g[*neighbor]);
      }
      catch (std::invalid_argument& e)
      {
        std::stringstream msg;
        msg << e.what() << ": "
          << "Focal vertex: " << g[*vd] << '\n'
          << "Neighbor vertex: " << g[*neighbor] << '\n'
        ;
        throw std::logic_error(msg.str());
      }
      assert(g[*vd].get_sil_frequencies().empty());
      assert(g[*neighbor].get_sil_frequencies().size() >= 2);
      //Move edges
      for (auto other_neighbor = neighbors.first; other_neighbor != neighbors.second; ++other_neighbor)
      {
        //No self loops
        if (neighbor == other_neighbor) continue;
        //No adding new edges
        if (edge(*neighbor, *other_neighbor, g).second) continue;
        //Add it. Because all edges between species are already present, new edges
        //will be between generations
        add_bundled_edge(
          *neighbor,
          *other_neighbor,
          sil_frequency_edge(1),
          g
        );
      }
      //Delete this vertex its edges
      boost::clear_vertex(*vd, g);
      break;
    }
  }

  remove_unconnected_empty_vertices(g);
  return g;
}
