#ifndef JOBO_PARAMETERS_H
#define JOBO_PARAMETERS_H

class jobo_parameters
{
public:
  jobo_parameters(
    const int n_loci,
    const int population_size
  );

  int get_n_loci() const { return m_n_loci; }
  int get_population_size() const { return m_population_size;}

private:
  int m_n_loci;
  int m_population_size;
};

bool operator==(const jobo_parameters& lhs, const jobo_parameters& rhs);
bool operator!=(const jobo_parameters& lhs, const jobo_parameters& rhs);

#endif // JOBO_PARAMETERS_H
