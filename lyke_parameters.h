#ifndef LYKE_PARAMETERS_H
#define LYKE_PARAMETERS_H

#include <iosfwd>
#include <string>
#include <iostream>

int create_random_seed() noexcept;

class lyke_parameters
{
public:
  lyke_parameters(
    const int    simulationruns = 10,
    const int    L = 10,
    const int    nGeneEco = 10,
    const double mu = 0.001,
    const double sigmaMut = 0.2,
    const int    popSize = 10,
    const double sigmac = 1.0,
    const double sigmaK = 5.0,
    const double alpha = 1.0,
    const double beta = 0.1,
    const int seed = create_random_seed()
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

  ///beta is decrease
  double get_beta() const noexcept;
  //double get_seed() const noexcept;
  std::string get_version() const noexcept {return "0.1";}

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
  int m_seed;
};

std::ostream& operator<<(std::ostream& os, const lyke_parameters p) noexcept;

///TODO: make local variabel one day
extern lyke_parameters g_parameters;

#endif // LYKE_PARAMETERS_H
