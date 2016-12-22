#ifndef KEWE_INDIVIDUAL_H
#define KEWE_INDIVIDUAL_H

#include <vector>
#include <random>
#include "kewe_fwd.h"

namespace kewe {

class individual
{
public:
  /// @param eco_trait the phenotypical ecological trait
  /// @param fem_pref the phenotypical female preference
  /// @param male_trait_phenotype the phenotypical male trait
  /// @param eco_trait_alleles the alleles coding for ecological trait
  /// @param fem_pref_alleles the alleles coding for female preference
  /// @param male_trait_alleles the alleles coding for male trait
  explicit individual(
    const double eco_trait = 0.0,
    const double fem_pref = 0.0,
    const double male_trait = 0.0,
    const std::vector<double>& eco_trait_loci = { 0.0 },
    const std::vector<double>& fem_pref_loci = { 0.0 },
    const std::vector<double>& male_trait_loci = { 0.0 }
  );

  ///Create an individual fuzzily
  individual(const simulation_parameters& parameters, std::mt19937& gen);

  // Make a new baby from mother m and father f
  void birth(
    const individual& m,
    const individual& f,
    const simulation_parameters& m_fem_pref,
    std::mt19937& gen
  );

  double get_eco_trait() const noexcept { return m_eco_trait; }
  double get_fem_pref() const noexcept { return m_fem_pref; }
  double get_male_trait() const noexcept { return m_male_trait; }

  const auto& get_eco_trait_loci() const noexcept { return m_eco_trait_loci; }
  const auto& get_fem_pref_loci() const noexcept { return m_fem_pref_loci; }
  const auto& get_male_trait_loci() const noexcept { return m_male_trait_loci; }

private:


  ///Ecological trait
  double m_eco_trait;

  ///Ecological trait loci
  std::vector<double> m_eco_trait_loci;

  ///Female preference
  double m_fem_pref;

  ///Female preference loci
  std::vector<double> m_fem_pref_loci;

  ///Male trait
  double m_male_trait;

  ///Male trait loci
  std::vector<double> m_male_trait_loci;

  void birth_haploid(
    const individual& m,
    const individual& f,
    const simulation_parameters& parameters,
    std::mt19937& gen
  );

  void birth_diploid(
    const individual& m,
    const individual& f,
    const simulation_parameters& parameters,
    std::mt19937& gen
  );

  friend bool operator==(const individual& lhs, const individual& rhs) noexcept;
};

void birth_diploid_trait(
  const double i,
  std::vector<double>& trait,
  double& avg_trait,
  const std::vector<double>& m_trait,
  const std::vector<double>& f_trait,
  const simulation_parameters& parameters,
  std::mt19937& gen
);

void birth_haploid_trait(
  const int i, //locus index
  std::vector<double>& trait,
  double& avg_trait,
  const std::vector<double>& m_trait,
  const std::vector<double>& f_trait,
  const simulation_parameters& parameters,
  std::mt19937& gen
);

///Create one offspring.
individual create_offspring(
  const individual& father,
  const individual& mother,
  const simulation_parameters& parameters,
  std::mt19937& rng_engine
);

///Creates just an individual for testing purposes
individual create_test_diploid() noexcept;

///Creates just an individual for testing purposes
individual create_test_haploid() noexcept;

bool operator==(const individual& lhs, const individual& rhs) noexcept;
bool operator!=(const individual& lhs, const individual& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const individual& i) noexcept;

} //~namespace kewe

#endif // KEWE_INDIVIDUAL_H
