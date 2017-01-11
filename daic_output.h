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
  ///Same order as DAISIE
  output(
    const double lambda_c = 0.0,
    const double mu = 0.0,
    const double k = 0.0,
    const double gamma = 0.0,
    const double lambda_a = 0.0,
    const double loglik = 0.0,
    const int df = 0,
    const bool conv = 0
  );

  auto get_lambda_c() const noexcept { return m_lambda_c; }
  auto get_mu() const noexcept { return m_mu; }
  auto get_k() const noexcept { return m_k; }
  auto get_gamma() const noexcept { return m_gamma; }
  auto get_lambda_a() const noexcept { return m_lambda_a; }
  auto get_loglik() const noexcept { return m_loglik; }
  auto get_df() const noexcept { return m_df; }
  auto get_conv() const noexcept { return m_conv; }

  private:
  ///Same order as DAISIE
  double m_lambda_c;
  double m_mu;
  double m_k;
  double m_gamma;
  double m_lambda_a;
  double m_loglik;
  int m_df;
  bool m_conv;

  friend bool operator==(const output& lhs, const output& rhs) noexcept;
  friend std::ostream& operator<<(std::ostream& os, const output& p) noexcept;
};

std::string get_output_header() noexcept;

std::string get_test_output_line() noexcept;

//"lambda_c","mu","K","gamma","lambda_a","loglik","df","conv"
//0.7,1.7,3.0,0.07,0.001,-27.3,5,0
std::vector<std::string> get_test_output_lines() noexcept;

output get_test_output() noexcept;

///Is the DAISIE output empty or default-constructed?
bool is_empty(const output& p) noexcept;

///Parse the line of a DAISIE output file with the estimated values
output read_output_from_string(const std::string& s);

///A file will look like this:
///"lambda_c","mu","K","gamma","lambda_a","loglik","df","conv"
///0.7,1.7,3.0,0.07,0.001,-27.3,5,0
///The second line will be parsed with 'read_output_from_string'
output read_output_from_file(const std::string& filename);

bool operator==(const output& lhs, const output& rhs) noexcept;

std::ostream& operator<<(std::ostream& os, const output& p) noexcept;

} //namespace daic

#endif // DAIC_OUTPUT_H
