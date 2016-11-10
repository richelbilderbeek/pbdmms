#ifndef PBD_PARAMETERS_H
#define PBD_PARAMETERS_H

namespace pbd {

class parameters
{
  public:
  parameters(
    const double birth_good,
    const double birth_incipient,
    const double completion,
    const double death_good,
    const double death_incipient,
    const double time,
    const int seed
  );

  const double m_birth_good;
  const double m_birth_incipient;
  const double m_completion;
  const double m_death_good;
  const double m_death_incipient;
  const double m_time;
  const int m_seed;
};

} //~namespace pbd

#endif // PBD_PARAMETERS_H
