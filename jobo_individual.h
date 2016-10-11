#ifndef JOBO_INDIVIDUAL_H
#define JOBO_INDIVIDUAL_H

#include <string>

namespace jobo {

class individual
{
public:
  individual(
    const int n_loci,
    const std::string& genotype = ""
  );

  int get_n_loci() const noexcept { return m_n_loci; }
  const std::string& get_genotype() const noexcept { return m_genotype;}

private:
  std::string m_genotype;
  int m_n_loci;
};

bool operator==(const individual& lhs, const individual& rhs) noexcept;
bool operator!=(const individual& lhs, const individual& rhs) noexcept;

} //~namespace jobo

#endif // JOBO_INDIVIDUAL_H
