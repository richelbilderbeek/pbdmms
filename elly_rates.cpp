#include "elly_rates.h"
#include <vector>

elly::rates::rates()
  : mclad{0.0}, mext{0.0}, mimm{0.0} , iext{0.0} , iclad{0.0} , iimm{0.0} , bextm{0.0},
    bexti{0.0}, bana{0.0}, bcladi{0.0} , bcladm{0.0} , ratesvector(11 , {0})
{
  ratesvector[0] = mclad;
  ratesvector[1] = mext;
  ratesvector[2] = mimm;
  ratesvector[3] = iext;
  ratesvector[4] = iclad;
  ratesvector[5] = iimm;
  ratesvector[6] = bextm;
  ratesvector[7] = bexti;
  ratesvector[8] = bana;
  ratesvector[9] = bcladi;
  ratesvector[10]= bcladm;

}
