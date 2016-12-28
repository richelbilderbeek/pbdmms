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
    const int pop_size = 0,
    const erasure_method erase_method = erasure_method::erase,
    const bool use_initialization_bug = true
  );


  auto get_erasure() const noexcept { return m_erasure; }
  auto get_pop_size() const noexcept { return m_pop_size; }
  bool get_use_initialization_bug() const noexcept { return m_use_initialization_bug; }

private:
  const erasure_method m_erasure;

  const int m_pop_size;
  bool m_use_initialization_bug;
};

void create_article_file(const std::string& filename);

/// A light version of the article
void create_golden_standard(const std::string& filename);

void create_testrun_file(const std::string& filename);

parameters readparameters(const std::string& filename);

erasure_method read_erasure_method(const std::string& filename);
int read_pop_size(const std::string& filename);
bool read_use_initialization_bug(const std::string& filename);

} //~namespace sado

#endif // SADO_PARAMETERS_H
