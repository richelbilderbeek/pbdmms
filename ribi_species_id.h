#ifndef SPECIES_ID_H
#define SPECIES_ID_H

#include <iosfwd>

class species_id
{
public:
  species_id();
  int get_id() const noexcept { return m_id; }
private:
  ///The ID the next species will get
  static int sm_next_id;

  ///The unique ID of a species
  int m_id;

  //friend std::ostream& operator<<(std::ostream& os, const species_id id) noexcept;
};

std::ostream& operator<<(std::ostream& os, const species_id id) noexcept;
bool operator==(const species_id& lhs, const species_id& rhs) noexcept;
bool operator!=(const species_id& lhs, const species_id& rhs) noexcept;

#endif // SPECIES_ID_H
