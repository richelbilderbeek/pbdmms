#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

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

public:
  int m_id_counter;

private:
  const double m_time_of_birth;
  const int    m_parent_id;
  const int    m_species_id;
  const double m_time_of_extinction;
};

void create_species();


} //~namespace elly
#endif // INDIVIDUAL_H
