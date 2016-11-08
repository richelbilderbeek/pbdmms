#ifndef KEWE_RESULTS_H
#define KEWE_RESULTS_H

#include <iostream>
#include <vector>

///For visualization
using histograms_in_time = std::vector<std::vector<double>>;

struct results
{
  histograms_in_time m_ecological_trait;
  histograms_in_time m_female_preference;
  histograms_in_time m_male_trait;
};

#endif // KEWE_RESULTS_H
