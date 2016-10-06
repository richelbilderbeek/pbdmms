#ifndef DISTANCER_sil_frequency_edge_H
#define DISTANCER_sil_frequency_edge_H

#include <boost/graph/graph_traits.hpp>

class sil_frequency_edge
{
public:
  sil_frequency_edge();

  explicit sil_frequency_edge(
    const int n_timesteps
  );

  ///The number of timesteps between two vertices
  int get_n_timesteps() const noexcept { return m_n_timesteps; }

private:
  int m_n_timesteps;
};

template <typename graph>
int get_edge_n_timesteps(
  const typename boost::graph_traits<graph>::edge_descriptor& ed,
  const graph& g
) noexcept
{
  return g[ed].get_n_timesteps();
}

#include <ostream>

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

#endif // DISTANCER_sil_frequency_edge_H
