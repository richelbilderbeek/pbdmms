#ifndef JOBO_PARAMETERS_H
#define JOBO_PARAMETERS_H

class jobo_parameters
{
public:
  jobo_parameters(
    const int n_loci
  );

  int get_n_loci() { return m_n_loci; }

private:
  int m_n_loci;
};


#endif // JOBO_PARAMETERS_H
