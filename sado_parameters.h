#ifndef SADO_PARAMETERS_H
#define SADO_PARAMETERS_H

#include "sado_erasure_method.h"
#include "sado_gausser.h"
#include "sado_gausser_implementation.h"
#include "sado_next_generation_method.h"
#include <iosfwd>
#include <string>

namespace sado
{

class parameters ///!OCLINT too many methods and fields indeed, will not fix this now
{
public:
  /// @param pop_size initial population size
  /// @param erase_method how to erase an individual. erasure::erase uses an
  /// erase method,
  ///   swap uses a much fater swap method
  /// @param use_init_bug keep in the bug from Van Doorn & Weissing
  ///   2001 in which the
  ///   initial population is initialized with 'pop_size' identical
  ///   individuals, that have a mismatch between genotype and phenotype
  explicit parameters(
      const double b,
      const double c,
      const int end_time,
      const erasure_method erase_method,
      const double eta,
      const gausser_implementation gausser_impl,
      const double histbinp,
      const double histbinq,
      const double histbinx,
      const next_generation_method next_gen_method,
      const std::string& output_filename,
      const int output_freq,
      const double p0,
      const int pop_size,
      const double q0,
      const double sc,
      const double se,
      const int seed,
      const double sk,
      const double sm,
      const double sq,
      const double sv,
      const bool use_init_bug,
      const double x0,
      const double at);

  /// Average number of offspring created per mating
  /// A value of 0.3 means that in 30% of all cases, 1 offspring is created
  auto get_b() const noexcept { return m_b; }

  /// Competetion intensity, a tuning parameter to make the population
  /// size stay more or less constant
  auto get_c() const noexcept { return m_c; }

  /// The number of overlapping generations this sim will last
  auto get_end_time() const noexcept { return m_end_time; }

  auto get_erasure() const noexcept { return m_erasure; }

  /// Choosiness of the female. If nonzero, she may choose not to mate
  /// (even if it would be the last chance to mate!)
  auto get_eta() const noexcept { return m_eta; }

  auto get_gausser_implementation() const noexcept
  {
    return m_gausser_impl;
  }

  const auto &get_gausser_sc() const noexcept { return m_gausser_sc; }
  const auto &get_gausser_se() const noexcept { return m_gausser_se; }
  const auto &get_gausser_sk() const noexcept { return m_gausser_sk; }
  const auto &get_gausser_sm() const noexcept { return m_gausser_sm; }
  const auto &get_gausser_sq() const noexcept { return m_gausser_sq; }

  auto get_histbinp() const noexcept { return m_histbinp; }
  auto get_histbinq() const noexcept { return m_histbinq; }
  auto get_histbinx() const noexcept { return m_histbinx; }

  /// Number of histogram bins
  int get_histw() const noexcept { return 50; }

  auto get_next_gen_method() const noexcept { return m_next_gen_method; }

  const auto &get_output_filename() const noexcept { return m_output_filename; }

  auto get_output_freq() const noexcept { return m_output_freq; }

  auto get_p0() const noexcept { return m_p0; }

  auto get_pop_size() const noexcept { return m_pop_size; }

  auto get_q0() const noexcept { return m_q0; }

  auto get_sc() const noexcept { return m_sc; }
  auto get_se() const noexcept { return m_se; }
  auto get_seed() const noexcept { return m_seed; }
  auto get_sk() const noexcept { return m_sk; }
  auto get_sm() const noexcept { return m_sm; }
  auto get_sq() const noexcept { return m_sq; }
  auto get_sv() const noexcept { return m_sv; }
  auto get_at() const noexcept { return m_at; }

  bool get_use_init_bug() const noexcept
  {
    return m_use_init_bug;
  }

  auto get_x0() const noexcept { return m_x0; }

  ///Set the competition intensity
  void set_c(const double c);

  ///Set the number of generations the simulation runs
  void set_end(const int end);

private:
  const double m_b;
  double m_c; //Used for determining the big-O complexity
  int m_end_time; //Used for determining the big-O complexity
  const erasure_method m_erasure;
  const double m_eta;
  const gausser_implementation m_gausser_impl;
  const gausser m_gausser_sc;
  const gausser m_gausser_se;
  const gausser m_gausser_sk;
  const gausser m_gausser_sm;
  const gausser m_gausser_sq;
  const double m_histbinp;
  const double m_histbinq;
  const double m_histbinx;
  const next_generation_method m_next_gen_method;
  const std::string m_output_filename;
  const int m_output_freq;
  const double m_p0;
  const int m_pop_size;
  const double m_q0;
  const double m_sc;
  const double m_se;
  const int m_seed;
  const double m_sk;
  const double m_sm;
  const double m_sq;
  const double m_sv;
  const bool m_use_init_bug;
  const double m_x0;
  const double m_at;

  friend bool operator==(const parameters& lhs, const parameters& rhs) noexcept;
};

void create_article_file(const std::string& filename);
parameters create_article_parameters();

void create_issue_264_file(const std::string& filename);
parameters create_issue_264_parameters();

/// A light version of the article
void create_golden_standard_file(const std::string& filename);

parameters create_golden_standard_parameters();

void create_profiling_file(const std::string& filename);
parameters create_profiling_parameters();

void create_testrun_file(const std::string& filename);

parameters create_testrun_parameters();

bool is_golden_standard(const parameters& p) noexcept;

parameters read_parameters(const std::string& filename);

double read_at(const std::string& filename);
double read_b(const std::string& filename);
double read_c(const std::string& filename);
int read_end_time(const std::string& filename);
erasure_method read_erasure_method(const std::string& filename);
double read_eta(const std::string& filename);
gausser_implementation read_gausser_implementation(const std::string& filename);
double read_histbinp(const std::string& filename);
double read_histbinq(const std::string& filename);
double read_histbinx(const std::string& filename);

next_generation_method read_next_gen_method(const std::string& filename);

std::string read_output_filename(const std::string& filename);

int read_output_freq(const std::string& filename);
double read_p0(const std::string& filename);
int read_pop_size(const std::string& filename);
double read_q0(const std::string& filename);
double read_sc(const std::string& filename);
double read_se(const std::string& filename);
int read_seed(const std::string& filename);
double read_sk(const std::string& filename);
double read_sm(const std::string& filename);
double read_sq(const std::string& filename);
double read_sv(const std::string& filename);
bool read_use_initialization_bug(const std::string& filename);
double read_x0(const std::string& filename);

void save_parameters(const parameters& p, const std::string& filename);

bool operator==(const parameters& lhs, const parameters& rhs) noexcept;
bool operator!=(const parameters& lhs, const parameters& rhs) noexcept;

std::ostream &operator<<(std::ostream &os, const parameters& p) noexcept;

} //~namespace sado

#endif // SADO_PARAMETERS_H
