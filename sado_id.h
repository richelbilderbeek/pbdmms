#ifndef SADO_ID_H
#define SADO_ID_H

#include <iosfwd>

namespace sado {

///Upon creation, created a unique ID
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

} //~namespace sado

#endif // SADO_ID_H
