#ifndef DAIC_OUTPUT_H
#define DAIC_OUTPUT_H

#include <string>
#include <vector>

///Here lambda c is an estimated parameter for per species cladogenesis on island,
/// mu is the estimated parameter for extinction rate per species, K is the value determining
/// the carrying capacity for a clade of species on the island, gamma is a per species rate of
/// immigration, lambda a is a per species rate of anagenesis.
namespace daic {

struct output
{
  double lambda_c;
  double mu;
  double k;
  double gamma;
  double lambda_a;
  double loglik;
  int df;
  bool conv;
};

std::string get_output_header() noexcept;

std::string get_test_output_line() noexcept;

///"lambda_c","mu","K","gamma","lambda_a","loglik","df","conv"
///0.767749320733306,1.77166095878803e-09,3.00003880280079,0.000750314431394791,0.000101614571827502,-27.3882792747044,5,0
std::vector<std::string> get_test_output_lines() noexcept;

output get_test_output() noexcept;

///Parse a line of a file. Example:
///"lambda_c","mu","K","gamma","lambda_a","loglik","df","conv"
///0.767749320733306,1.77166095878803e-09,3.00003880280079,0.000750314431394791,0.000101614571827502,-27.3882792747044,5,0
output read_output_from_string(const std::string& s);

///A file will look like this:
///"lambda_c","mu","K","gamma","lambda_a","loglik","df","conv"
///0.767749320733306,1.77166095878803e-09,3.00003880280079,0.000750314431394791,0.000101614571827502,-27.3882792747044,5,0
///The second line will be parsed with 'read_output_from_string'
output read_output_from_file(const std::string& filename);

bool operator==(const output& lhs, const output& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const output& p) noexcept;

} //namespace daic

#endif // DAIC_OUTPUT_H
