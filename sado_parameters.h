#ifndef SADO_PARAMETERS_H
#define SADO_PARAMETERS_H

#include <string>
#include "sado_erasure_method.h"

namespace sado {

class parameters
{
public:

  /// @param pop_size initial population size
  /// @param erase_method how to erase an individual. erasure::erase uses an erase method,
  ///   swap uses a much fater swap method
  /// @param use_initialization_bug keep in the bug from Van Doorn & Weissing 2001 in which the
  ///   initial population is initialized with differing geno- and phenotypes
  parameters(
    const double b,
    const double c,
    const erasure_method erase_method, // = erasure_method::erase,
    const double eta,
    const double histbinp,
    const double histbinq,
    const double histbinx,
    const std::string& output_filename,
    const double p0,
    const int pop_size,
    const double q0,
    const int seed,
    const bool use_initialization_bug,
    const double x0
  );

  ///Average number of offspring created per mating
  ///A value of 0.3 means that in 30% of all cases, 1 offspring is created
  auto get_b() const noexcept { return m_b; }

  ///Competetion intensity, a tuning parameter to make the population
  ///size stay more or less constant
  auto get_c() const noexcept { return m_c; }

  ///Choosiness of the female. If nonzero, she may choose not to mate
  /// (even if it would be the last chance to mate!)
  auto get_eta() const noexcept { return m_eta; }

  auto get_erasure() const noexcept { return m_erasure; }

  auto get_histbinp() const noexcept { return m_histbinp; }
  auto get_histbinq() const noexcept { return m_histbinq; }
  auto get_histbinx() const noexcept { return m_histbinx; }

  ///Number of histogram bins
  int get_histw() const noexcept { return 50; }

  const auto& get_output_filename() const noexcept { return m_output_filename; }

  auto get_p0() const noexcept { return m_p0; }

  auto get_pop_size() const noexcept { return m_pop_size; }

  auto get_q0() const noexcept { return m_q0; }

  auto get_seed() const noexcept { return m_seed; }

  bool get_use_initialization_bug() const noexcept { return m_use_initialization_bug; }

  auto get_x0() const noexcept { return m_x0; }

private:

  const double m_b;
  const double m_c;
  const erasure_method m_erasure;
  const double m_eta;
  const double m_histbinp;
  const double m_histbinq;
  const double m_histbinx;
  const std::string m_output_filename;
  const double m_p0;
  const int m_pop_size;
  const double m_q0;
  const int m_seed;
  const bool m_use_initialization_bug;
  const double m_x0;
};

void create_article_file(const std::string& filename);

/// A light version of the article
void create_golden_standard_file(const std::string& filename);

parameters create_golden_standard();

void create_profiling_file(const std::string& filename);

void create_testrun_file(const std::string& filename);

bool is_golden_standard(const parameters& p) noexcept;

parameters readparameters(const std::string& filename);

double read_b(const std::string& filename);
double read_c(const std::string& filename);
erasure_method read_erasure_method(const std::string& filename);
double read_eta(const std::string& filename);

double read_histbinp(const std::string& filename);
double read_histbinq(const std::string& filename);
double read_histbinx(const std::string& filename);

std::string read_output_filename(const std::string& filename);
double read_p0(const std::string& filename);
int read_pop_size(const std::string& filename);
double read_q0(const std::string& filename);
int read_seed(const std::string& filename);
bool read_use_initialization_bug(const std::string& filename);
double read_x0(const std::string& filename);

bool operator==(const parameters& lhs, const parameters& rhs) noexcept;

} //~namespace sado

#endif // SADO_PARAMETERS_H
