#include "ribi_sil_frequency_phylogeny.h"
#include <iostream>
#include <boost/graph/graphviz.hpp>
#include "ribi_sil_frequency_vertex_writer.h"
#include "ribi_sil_frequency_edge_writer.h"

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

std::ostream& ribi::operator<<(std::ostream& os, const sil_frequency_phylogeny& g) noexcept
{
  boost::write_graphviz(os, g,
    sil_frequency_vertex_writer<sil_frequency_phylogeny>(g),
    sil_frequency_edge_writer<sil_frequency_phylogeny>(g)
  );
  return os;
}

