#ifndef LYKE_PARAMETERS_H
#define LYKE_PARAMETERS_H

class lyke_parameters
{
public:
  lyke_parameters(
    const int    simulationruns = 20,
    const int    L = 20,
    const int    nGeneEco = 10,
    const double mu = 0.001,
    const double sigmaMut = 0.2,
    const int    popSize = 5000,
    const double sigmac = 1.0,
    const double sigmaK = 5.0,
    const double alpha = 1.0,
    const double beta = 0.1
  );

  int get_simulationruns() const noexcept;
  int get_L() const noexcept;
  int get_nGeneEco() const noexcept;
  double get_mu() const noexcept;
  double get_sigmaMut() const noexcept;
  int get_popSize() const noexcept;
  double get_sigmac() const noexcept;
  double get_sigmaK() const noexcept;
  double get_alpha() const noexcept;
  double get_beta() const noexcept;


private:
  int    m_simulationruns;
  int    m_L;
  int    m_nGeneEco;
  double m_mu;
  double m_sigmaMut;
  int    m_popSize;
  double m_sigmac;
  double m_sigmaK;
  double m_alpha;
  double m_beta;
};

#endif // LYKE_PARAMETERS_H
