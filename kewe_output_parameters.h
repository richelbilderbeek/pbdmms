#ifndef KEWE_OUTPUT_PARAMETERS_H
#define KEWE_OUTPUT_PARAMETERS_H

#include <string>

namespace kewe {

struct output_parameters
{
  int outputfreq = 10;                            // output frequency
  int histw = 50;                                 // Number of steps in the histograms
  double histbinx = 0.1;                          // stepsize of histogram x
  double histbinp = 0.1;                          // stepsize of histogram p
  double histbinq = 0.1;                          // stepsize of histogram q
  std::string outputfilename = "defaultresults";
  std::string ltt_plot_filename = "defaultlttplot";
  bool is_silent;
};

std::ostream& operator<<(std::ostream& os, const output_parameters p) noexcept;

} //~namespace kewe

#endif // KEWE_OUTPUT_PARAMETERS_H
