#include "ribi_sil_frequency_phylogeny.h"
#include <iostream>
#include <boost/graph/graphviz.hpp>
#include "ribi_sil_frequency_vertex_writer.h"
#include "ribi_sil_frequency_edge_writer.h"
#include "ribi_results.h"
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"

void ribi::add_sil_frequency_edge(
  const sil_frequency_edge edge,
  const sil_frequency_vertex_descriptor& from,
  const sil_frequency_vertex_descriptor& to,
  sil_frequency_phylogeny& g
)
{
  const auto ed = boost::add_edge(from, to, g);
  assert(ed.second);
  g[ed.first] = edge;
}

ribi::sil_frequency_phylogeny ribi::get_test_sil_frequency_phylogeny_1() noexcept
{
  sil_frequency_phylogeny p;
  /*
             2
            / \
   Past 0--1   4--5 Present
            \ /
             3

   --+---+-------------------
   # | t | fs (SIL + f)
   --+---+-------------------
   0 | 1 | {{000,2}}
   1 | 2 | {{001,2}}
   2 | 3 | {{010,1}}
   3 | 3 | {{011,1}}
   4 | 4 | {{100,2}}
   5 | 5 | {{101,2}}
  */
  const sil sil0{create_sil("000")};
  const sil sil1{create_sil("001")};
  const sil sil2{create_sil("010")};
  const sil sil3{create_sil("011")};
  const sil sil4{create_sil("100")};
  const sil sil5{create_sil("101")};
  const std::map<sil,int>& sfs0 = {{sil0, 2}};
  const std::map<sil,int>& sfs1 = {{sil1, 2}};
  const std::map<sil,int>& sfs2 = {{sil2, 1}};
  const std::map<sil,int>& sfs3 = {{sil3, 1}};
  const std::map<sil,int>& sfs4 = {{sil4, 2}};
  const std::map<sil,int>& sfs5 = {{sil5, 2}};
  const auto vd0 = boost::add_vertex(sil_frequency_vertex(sfs0, 1),p);
  const auto vd1 = boost::add_vertex(sil_frequency_vertex(sfs1, 2),p);
  const auto vd2 = boost::add_vertex(sil_frequency_vertex(sfs2, 3),p);
  const auto vd3 = boost::add_vertex(sil_frequency_vertex(sfs3, 3),p);
  const auto vd4 = boost::add_vertex(sil_frequency_vertex(sfs4, 4),p);
  const auto vd5 = boost::add_vertex(sil_frequency_vertex(sfs5, 5),p);
  add_sil_frequency_edge(sil_frequency_edge(1), vd0, vd1, p);
  add_sil_frequency_edge(sil_frequency_edge(1), vd1, vd2, p);
  add_sil_frequency_edge(sil_frequency_edge(1), vd1, vd3, p);
  add_sil_frequency_edge(sil_frequency_edge(1), vd2, vd4, p);
  add_sil_frequency_edge(sil_frequency_edge(1), vd3, vd4, p);
  add_sil_frequency_edge(sil_frequency_edge(1), vd4, vd5, p);
  return p;
}

ribi::sil_frequency_phylogeny ribi::get_test_sil_frequency_phylogeny_2() noexcept //!OCLINT I consider this readable
{
  sil_frequency_phylogeny p;
  /*
             2--4
            /    \
   Past 0--1      6--7 Present
            \    /
             3--5

   --+---+-------------------
   # | t | fs (SIL + f)
   --+---+-------------------
   0 | 1 | {{000,2}}
   1 | 2 | {{001,2}}
   2 | 3 | {{010,1}}
   3 | 3 | {{011,1}}
   4 | 4 | {{010,1}}
   5 | 4 | {{011,1}}
   6 | 5 | {{100,2}}
   7 | 6 | {{101,2}}

  */
  const sil sil0{create_sil("000")};
  const sil sil1{create_sil("001")};
  const sil sil2{create_sil("010")};
  const sil sil3{create_sil("011")};
  const sil sil4{create_sil("010")};
  const sil sil5{create_sil("011")};
  const sil sil6{create_sil("100")};
  const sil sil7{create_sil("101")};
  const std::map<sil,int>& sfs0 = {{sil0, 2}};
  const std::map<sil,int>& sfs1 = {{sil1, 2}};
  const std::map<sil,int>& sfs2 = {{sil2, 1}};
  const std::map<sil,int>& sfs3 = {{sil3, 1}};
  const std::map<sil,int>& sfs4 = {{sil4, 1}};
  const std::map<sil,int>& sfs5 = {{sil5, 1}};
  const std::map<sil,int>& sfs6 = {{sil6, 2}};
  const std::map<sil,int>& sfs7 = {{sil7, 2}};
  const auto vd0 = boost::add_vertex(sil_frequency_vertex(sfs0, 1),p);
  const auto vd1 = boost::add_vertex(sil_frequency_vertex(sfs1, 2),p);
  const auto vd2 = boost::add_vertex(sil_frequency_vertex(sfs2, 3),p);
  const auto vd3 = boost::add_vertex(sil_frequency_vertex(sfs3, 3),p);
  const auto vd4 = boost::add_vertex(sil_frequency_vertex(sfs4, 4),p);
  const auto vd5 = boost::add_vertex(sil_frequency_vertex(sfs5, 4),p);
  const auto vd6 = boost::add_vertex(sil_frequency_vertex(sfs6, 5),p);
  const auto vd7 = boost::add_vertex(sil_frequency_vertex(sfs7, 6),p);
  add_sil_frequency_edge(sil_frequency_edge(1), vd0, vd1, p);
  add_sil_frequency_edge(sil_frequency_edge(1), vd1, vd2, p);
  add_sil_frequency_edge(sil_frequency_edge(1), vd1, vd3, p);
  add_sil_frequency_edge(sil_frequency_edge(1), vd2, vd4, p);
  add_sil_frequency_edge(sil_frequency_edge(1), vd3, vd5, p);
  add_sil_frequency_edge(sil_frequency_edge(1), vd4, vd6, p);
  add_sil_frequency_edge(sil_frequency_edge(1), vd5, vd6, p);
  add_sil_frequency_edge(sil_frequency_edge(1), vd6, vd7, p);
  return p;
}

void ribi::move_sil_connection(
  const sil_frequency_vertex_descriptor from,
  const sil_frequency_vertex_descriptor to,
  sil_frequency_phylogeny& g
)
{
  // pre_from -+- from -+- post_from
  // pre_from -+- from -+- post_from
  // pre_from -+- from -+- post_from
  //           |        |
  //           v        v
  // pre_to ------- to --- post_to
  move_sil_connection_pre(from, to, g);
  move_sil_connection_post(from, to, g);
  boost::clear_vertex(from, g);
  assert(boost::degree(from, g) == 0);
}

void ribi::move_sil_connection_post(
  const sil_frequency_vertex_descriptor from,
  const sil_frequency_vertex_descriptor to,
  sil_frequency_phylogeny& g
)
{
  /*
          -+- post_from
         /-+- post_from
     from -+- post_from
           |
           v
     to ------- post_to
  */
  for (const auto post_from: get_younger(from, g))
  {
    const auto post_tos = get_younger(to, g);
    assert(post_tos.size() == 1);
    const auto post_to = post_tos.back();
    const auto erp_from = boost::edge(post_from, from, g); //edge result pair
    const auto erp_to = boost::edge(post_to, to, g); //edge result pair
    assert(erp_from.second); // Edge must exist
    assert(erp_to.second); // Edge must exist
    move_sil_frequencies(g[erp_from.first], g[erp_to.first]);
  }
}

void ribi::move_sil_connection_pre(
  const sil_frequency_vertex_descriptor from,
  const sil_frequency_vertex_descriptor to,
  sil_frequency_phylogeny& g
)
{
  /*
     pre_from -+-
     pre_from -+-\
     pre_from -+- from
               |
               v
     pre_to ------- to
  */
  for (const auto pre_from: get_older(from, g))
  {
    const auto pre_tos = get_older(to, g);
    assert(pre_tos.size() == 1);
    const auto pre_to = pre_tos.back();
    const auto erp_from = boost::edge(pre_from, from, g); //edge result pair
    const auto erp_to = boost::edge(pre_to, to, g); //edge result pair
    assert(erp_from.second); // Edge must exist
    assert(erp_to.second); // Edge must exist
    move_sil_frequencies(g[erp_from.first], g[erp_to.first]);
  }
}

void ribi::move_sil_connections(
  const sil_frequency_vertex_descriptors& froms,
  const sil_frequency_vertex_descriptor to,
  sil_frequency_phylogeny& g
)
{
  for (const auto from: froms)
  {
    move_sil_connection(from, to, g);
  }
}


void ribi::move_sil_frequencies(
  const sil_frequency_vertex_descriptors& froms,
  const sil_frequency_vertex_descriptor to,
  sil_frequency_phylogeny& g
)
{
  for (const auto from: froms)
  {
    move_sil_frequencies(g[from], g[to]);
  }
}

void ribi::save_to_png(
  const ribi::sil_frequency_phylogeny& p,
  const std::string& filename
)
{
  const std::string dot_filename{filename + ".dot"};
  const std::string png_filename{filename + ".png"};
  const std::string svg_filename{filename + ".svg"};
  {
    std::ofstream f(dot_filename);
    f << p;
  }
  convert_dot_to_svg(dot_filename, svg_filename);
  convert_svg_to_png(svg_filename, png_filename);
}


std::ostream& ribi::operator<<(std::ostream& os, const sil_frequency_phylogeny& g) noexcept
{
  boost::write_graphviz(os, g,
    sil_frequency_vertex_writer<sil_frequency_phylogeny>(g),
    sil_frequency_edge_writer<sil_frequency_phylogeny>(g)
  );
  return os;
}

