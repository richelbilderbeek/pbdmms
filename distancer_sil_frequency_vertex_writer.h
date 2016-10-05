#ifndef DISTANCER_sil_frequency_vertex_writer_H
#define DISTANCER_sil_frequency_vertex_writer_H

#include <cassert>
#include <sstream>
#include <ostream>

#include "distancer_sil_frequency_vertex.h"

template <
  typename graph
>
class sil_frequency_vertex_writer {
public:
  sil_frequency_vertex_writer(graph g) : m_g{g}
  {

  }
  template <class vertex_descriptor>
  void operator()(
    std::ostream& out,
    const vertex_descriptor& vd
  ) const noexcept
  {
    //SIL frequencies
    std::stringstream str;
    const auto fs = m_g[vd].get_sil_frequencies();
    for (const auto p: fs) {
      str << p.first << "(" << p.second << "),";
    }
    std::string fss = str.str();
    if (!fss.empty()) fss.pop_back(); //Get rid of trailing comma

    //Style string
    out
      << "["
      << "label=\""
      #ifndef NDEBUG
      << m_g[vd].get_id() << ": "
      #endif
      << m_g[vd].get_time()
      << ": "
      << fss
      << "\""
    ;
    switch (m_g[vd].get_style())
    {
      case sil_frequency_vertex_style::unknown  : out << " style=dotted"; break;
      case sil_frequency_vertex_style::incipient: out << " style=dashed"; break;
      case sil_frequency_vertex_style::good     : out << " style=bold"  ; break;
    }
    out << "]";
  }
private:
  graph m_g;
};


#endif // DISTANCER_sil_frequency_vertex_writer_H
