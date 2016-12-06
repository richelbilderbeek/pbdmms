#ifndef SPECIE_H
#define SPECIE_H

namespace elly{

class specie
{
public:
  specie(
    double time_of_birth,
    int parent_id,
    int specie_id,
    double time_of_extinction);



private:
  const double time_of_birth;
  const int parent_id;
  const int specie_id;
  const double time_of_extinction;
};
} //~namespace elly
#endif // SPECIE_H
