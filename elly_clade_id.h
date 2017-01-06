#ifndef ELLY_CLADE_ID_H
#define ELLY_CLADE_ID_H

#include <iosfwd>

namespace elly {

///Upon creation, created a unique ID
class clade_id
{
public:

  int get_id() const noexcept { return m_id; }

private:

  ///Private constructor: the specied_id must be created by create_new_clade_id,
  ///to explicitly show construction of it
  clade_id(const int id);
  friend clade_id create_new_clade_id() noexcept;
  friend clade_id create_null_clade_id() noexcept;

  ///This ID
  int m_id;
};

clade_id create_new_clade_id() noexcept;

clade_id create_null_clade_id() noexcept;

std::ostream& operator<<(std::ostream& os, const clade_id& id) noexcept;

bool operator==(const clade_id lhs, const clade_id rhs) noexcept;
bool operator!=(const clade_id lhs, const clade_id rhs) noexcept;
bool operator<(const clade_id lhs, const clade_id rhs) noexcept;


} //~namespace elly

#endif // ELLY_CLADE_ID_H
