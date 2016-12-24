#ifndef KEWE_OUTPUT_PARAMETERS_H
#define KEWE_OUTPUT_PARAMETERS_H

#include <string>

namespace kewe {

struct output_parameters
{
  /// output frequency, is zero if only at end
  int outputfreq = 10;

  int histw = 50;                                 // Number of steps in the histograms
  double histbinx = 0.1;                          // stepsize of histogram x
  double histbinp = 0.1;                          // stepsize of histogram p
  double histbinq = 0.1;                          // stepsize of histogram q
  std::string outputfilename = "defaultresults";
  std::string ltt_plot_filename = "defaultlttplot";
  bool is_silent;
};

bool is_valid(const output_parameters& p) noexcept;

///Read the output_parameters from a file
output_parameters read_output_parameters(const std::string& filename);

std::ostream& operator<<(std::ostream& os, const output_parameters p) noexcept;

} //~namespace kewe

#endif // KEWE_OUTPUT_PARAMETERS_H
