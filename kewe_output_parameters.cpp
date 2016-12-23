#include "kewe_output_parameters.h"

#include <iostream>

std::ostream& kewe::operator<<(std::ostream& os, const output_parameters p) noexcept
{
  os
    << "outputfrequency: " << p.outputfreq << '\n'
    << "histw: " << p.histw << '\n'
    << "histbinx: " << p.histbinx << '\n'
    << "histbinp: " << p.histbinp << '\n'
    << "histbinq: " << p.histbinq << '\n'
    << "outputfilename: " << p.outputfilename << '\n'
    << "ltt_plot_filename: " << p.ltt_plot_filename << '\n'
    << "is silent: " << static_cast<bool>(p.is_silent);

  return os;
}
