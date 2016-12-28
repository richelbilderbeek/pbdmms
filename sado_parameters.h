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
    const erasure_method erase_method, // = erasure_method::erase,
    const std::string& output_filename,
    const int pop_size,
    const int seed,
    const bool use_initialization_bug
  );


  auto get_erasure() const noexcept { return m_erasure; }

  ///Number of histogram bins
  int get_histw() const noexcept { return 50; }

  const auto& get_output_filename() const noexcept { return m_output_filename; }
  auto get_pop_size() const noexcept { return m_pop_size; }

  auto get_seed() const noexcept { return m_seed; }

  bool get_use_initialization_bug() const noexcept { return m_use_initialization_bug; }

private:

  const erasure_method m_erasure;
  const std::string m_output_filename;
  const int m_pop_size;
  const int m_seed;
  const bool m_use_initialization_bug;
};

void create_article_file(const std::string& filename);

/// A light version of the article
void create_golden_standard_file(const std::string& filename);

parameters create_golden_standard();

void create_testrun_file(const std::string& filename);

bool is_golden_standard(const parameters& p) noexcept;

parameters readparameters(const std::string& filename);

erasure_method read_erasure_method(const std::string& filename);
std::string read_output_filename(const std::string& filename);
int read_pop_size(const std::string& filename);
int read_seed(const std::string& filename);
bool read_use_initialization_bug(const std::string& filename);

bool operator==(const parameters& lhs, const parameters& rhs) noexcept;

} //~namespace sado

#endif // SADO_PARAMETERS_H
