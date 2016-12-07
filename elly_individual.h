#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H
#include <stdexcept>

namespace elly{

class individual
{
public:
  individual(
    double time_of_birth,
    int parent_id,
    int species_id,
    double time_of_extinction,
    int id_counter = 0
  );

  void iterate_species_id();
  int get_species_id() const noexcept {  return m_species_id; }
  int get_parent_id() const noexcept {  return m_parent_id;  }
  double get_time_of_birth() const noexcept {  return m_time_of_birth; }
  double get_time_of_extinction() const noexcept {  return m_time_of_extinction; }
  int get_id_counter();

private:
  const double m_time_of_birth;
  const int    m_parent_id;
  const int    m_species_id;
  const double m_time_of_extinction;
  int m_id_counter;
};

void create_species();


} //~namespace elly
#endif // INDIVIDUAL_H
