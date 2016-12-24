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
    << "is silent: " << p.is_silent;

  return os;
}

bool kewe::is_valid(const output_parameters& p) noexcept
{
  return p.outputfreq >= 0
    && p.histw > 1
    && p.histbinx > 0.0
    && p.histbinp > 0.0
    && p.histbinq > 0.0
    && !p.outputfilename.empty()
    && !p.ltt_plot_filename.empty()
  ;
}

kewe::output_parameters kewe::read_output_parameters(const std::string& /* filename */)
{
  return output_parameters(

  );
}
