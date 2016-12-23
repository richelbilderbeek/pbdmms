#ifndef KEWE_GAUSSERS_H
#define KEWE_GAUSSERS_H

#include "kewe_simulation_parameters.h"
#include "kewe_gausser.h"

namespace kewe {

class gaussers
{
public:
  gaussers(const simulation_parameters& p);
private:
  const gausser m_mate_spec_eco;
  const gausser m_mate_spec_mate;
};

} //~namespace kewe


#endif // KEWE_GAUSSERS_H
