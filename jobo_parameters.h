#ifndef JOBO_PARAMETERS_H
#define JOBO_PARAMETERS_H

namespace jobo {

class parameters
{
public:
  parameters(
    const int n_loci,
    const int population_size
  );

  int get_n_loci() const noexcept { return m_n_loci; }
  int get_population_size() const noexcept { return m_population_size;}

private:
  int m_n_loci;
  int m_population_size;
};

bool operator==(const parameters& lhs, const parameters& rhs) noexcept;
bool operator!=(const parameters& lhs, const parameters& rhs) noexcept;

} //~namespace jobo

#endif // JOBO_PARAMETERS_H
