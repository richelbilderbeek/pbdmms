#ifndef DISTANCER_sil_frequency_vertex_H
#define DISTANCER_sil_frequency_vertex_H

#include <map>
#include <boost/graph/graph_traits.hpp>
#include "distancer_sil.h"

enum class sil_frequency_vertex_style { unknown, good, incipient };

class sil_frequency_vertex
{
public:
  using style = sil_frequency_vertex_style;
  sil_frequency_vertex();

  explicit sil_frequency_vertex(
    const std::map<sil,int>& sil_frequencies,
    const int time
  );

  void clear_sil_frequencies() noexcept { m_sil_frequencies.clear(); }

  style get_style() const noexcept { return m_style; }

  #ifndef NDEBUG
  int get_id() const noexcept { return m_id; }
  #endif // NDEBUG

  ///The SIL frequencies
  const std::map<sil,int>& get_sil_frequencies() const noexcept { return m_sil_frequencies; }

  ///The timepoint
  int get_time() const noexcept { return m_time; }

  void set_style(const style& s) noexcept { m_style = s; }

private:
  #ifndef NDEBUG
  int m_id; ///Unique ID
  static int s_m_next_id;
  #endif // NDEBUG

  ///The SIL frequencies
  std::map<sil,int> m_sil_frequencies;

  ///What kind of vertex is this?
  style m_style;

  ///The timestep
  int m_time;

  friend void move_sil_frequencies(sil_frequency_vertex& from, sil_frequency_vertex& to);
};

int count_n_possible_species(
  const sil_frequency_vertex& vertex,
  const int max_genetic_distance
) noexcept;

///Get the SIL frequencies as a std::string
std::string get_sil_frequencies_str(const sil_frequency_vertex& v) noexcept;

///Move all SIL frequences from 'from' to 'to'
void move_sil_frequencies(sil_frequency_vertex& from, sil_frequency_vertex& to);

std::ostream& operator<<(std::ostream& os, const sil_frequency_vertex& v) noexcept;

#endif // DISTANCER_sil_frequency_vertex_H
