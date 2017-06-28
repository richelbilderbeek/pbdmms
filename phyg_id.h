#ifndef PHYG_ID_H
#define PHYG_ID_H

#include <iosfwd>

namespace phyg {

///Upon creation, creates a unique ID, to be used for an individual
///For species, use the 'species_id' class
class id
{
public:
  int get_id() const noexcept { return m_id; }

private:

  ///Private constructor: the specied_id must be created by create_new_id,
  ///to explicitly show construction of it
  id(const int id);
  friend id create_new_id() noexcept;
  friend id create_null_id() noexcept;
  friend id create_negative_of(const id id);

  ///This ID
  int m_id;
};

bool operator==(const id lhs, const id rhs) noexcept;
bool operator!=(const id lhs, const id rhs) noexcept;
bool operator<(const id lhs, const id rhs) noexcept;
bool operator>(const id lhs, const id rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const id id) noexcept;

id create_new_id() noexcept;

id create_null_id() noexcept;

} //~namespace phyg

#endif // PHYG_ID_H
