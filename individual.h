#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

namespace elly{

class individual
{
public:
  individual(
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
#endif // INDIVIDUAL_H
