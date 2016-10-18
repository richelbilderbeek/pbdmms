#ifndef RIBI_SIL_FREQUENCY_EDGE_WRITER_H
#define RIBI_SIL_FREQUENCY_EDGE_WRITER_H

#include "ribi_sil_frequency_edge.h"
#include <ostream>

namespace ribi {

template <
  typename graph
>
class sil_frequency_edge_writer {
public:
  sil_frequency_edge_writer(
    graph g
  ) : m_g{g}
  {

  }
  template <class vertex_descriptor>
  void operator()(
    std::ostream& out,
    const vertex_descriptor& ed
  ) const noexcept {
    out
      << "[label=\""
      << m_g[ed].get_n_timesteps()
      << "\"]"
    ;
  }
private:
  graph m_g;
};

} //~namespace ribi


#endif // RIBI_SIL_FREQUENCY_EDGE_WRITER_H
