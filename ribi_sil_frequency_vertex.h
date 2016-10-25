#ifndef DISTANCER_sil_frequency_vertex_H
#define DISTANCER_sil_frequency_vertex_H

#include <map>
#include <boost/graph/graph_traits.hpp>
#include "ribi_sil.h"

namespace ribi {

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

  int get_id() const noexcept { return m_id; }

  ///The SIL frequencies
  const std::map<sil,int>& get_sil_frequencies() const noexcept { return m_sil_frequencies; }

  ///The timepoint
  int get_time() const noexcept { return m_time; }

  void set_style(const style& s) noexcept { m_style = s; }

private:
  int m_id; ///Unique ID
  static int s_m_next_id; //!OCLINT static needed to track number of constructions

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

///Merges two SIL frequency tallies
///Examples:
/// * lhs = {{00, 1}}, rhs = {{11, 2}} -> {{00, 1}, {11, 2}}
/// * lhs = {{00, 1}}, rhs = {{00, 2}} -> {{00, 3}}
/// * lhs = {{00, 1}}, rhs = {{00, 3}} -> {{00, 4}}
/// * lhs = {{00, 1}}, rhs = {{00, 2}, {11, 1}} -> {{00, 3}, {11, 1}}
std::map<sil,int> merge_sil_frequencies(
  const std::map<sil,int>& lhs,
  const std::map<sil,int>& rhs
);


///Move all SIL frequences from 'from' to 'to'
///Clears 'from'
void move_sil_frequencies(sil_frequency_vertex& from, sil_frequency_vertex& to);

std::string to_str(const sil_frequency_vertex_style s) noexcept;

///Returns true if the content is the same
bool operator==(
  const sil_frequency_vertex& lhs,
  const sil_frequency_vertex& rhs
) noexcept;


std::ostream& operator<<(std::ostream& os, const sil_frequency_vertex_style& s) noexcept;

std::ostream& operator<<(std::ostream& os, const sil_frequency_vertex& v) noexcept;


} //~namespace ribi

#endif // DISTANCER_sil_frequency_vertex_H
