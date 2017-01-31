#include "ribi_sil_frequency_vertex_writer.h"

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <boost/test/unit_test.hpp>

#include "ribi_sil_frequency_phylogeny.h"

using namespace ribi;

BOOST_AUTO_TEST_CASE(test_sil_frequency_vertex_writer)
{
  //ribi::sil_frequency_vertex_writer is called by
  //std::ostream& ribi::operator<<(std::ostream& os, const sil_frequency_phylogeny& g) noexcept

  //Create a phylogeny with all three vertex styles
  sil_frequency_phylogeny p;
  const sil sil0{create_sil("000")};
  const sil sil1{create_sil("001")};
  const sil sil2{create_sil("010")};
  const std::map<sil,int>& sfs0 = {{sil0, 2}};
  const std::map<sil,int>& sfs1 = {{sil1, 2}};
  const std::map<sil,int>& sfs2 = {{sil2, 1}};
  sil_frequency_vertex sfv0(sfs0, 1);
  sil_frequency_vertex sfv1(sfs1, 1);
  sil_frequency_vertex sfv2(sfs2, 1);
  sfv0.set_style(sil_frequency_vertex_style::good);
  sfv1.set_style(sil_frequency_vertex_style::incipient);
  sfv2.set_style(sil_frequency_vertex_style::unknown);
  boost::add_vertex(sfv0,p);
  boost::add_vertex(sfv1,p);
  boost::add_vertex(sfv2,p);

  std::stringstream s;
  s << p;
  BOOST_CHECK(!s.str().empty());
}


#pragma GCC diagnostic pop

