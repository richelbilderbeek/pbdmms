#include "lyke_utils.h"

void clip_range(double &val, const double min, const double max)
{
  if(val < min)
  {
    val = min;
  }
  else if(val > max)
  {
    val = max;
  }
}
