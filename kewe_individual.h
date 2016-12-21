#ifndef KEWE_INDIVIDUAL_H
#define KEWE_INDIVIDUAL_H

#include <vector>
#include <random>
#include "kewe_fwd.h"

namespace kewe {

class individual
{
  private:
    ///Ecological trait loci
    std::vector<double> m_X;

    ///Female preference loci
    std::vector<double> m_P;

    ///Male trait loci
    std::vector<double> m_Q;

    ///Ecological trait
    double m_x;

    ///Female preference
    double m_p;

    ///Male trait
    double m_q;

    void birth_haploid(
        const individual& m,
        const individual& f,
        const parameters& parameters,
        std::mt19937& gen
        );

    void birth_diploid(
        const individual& m,
        const individual& f,
        const parameters& parameters,
        std::mt19937& gen
        );

    void birth_haploid_trait(
        const int i, //locus index
        std::vector<double>& trait,
        double& avg_trait,
        const std::vector<double>& m_trait,
        const std::vector<double>& f_trait,
        const parameters& parameters,
        std::mt19937& gen
        );

    void birth_diploid_trait(
      const double i,
      std::vector<double>& trait,
      double& avg_trait,
      const std::vector<double>& m_trait,
      const std::vector<double>& f_trait,
      const parameters& parameters,
      std::mt19937& gen
  );

  public:
    /// @param eco_trait the phenotypical ecological trait
    /// @param fem_pref the phenotypical female preference
    /// @param male_trait_phenotype the phenotypical male trait
    /// @param eco_trait_alleles the alleles coding for ecological trait
    /// @param fem_pref_alleles the alleles coding for female preference
    /// @param male_trait_alleles the alleles coding for male trait
    individual(
      const double eco_trait,
      const double fem_pref,
      const double male_trait,
      const std::vector<double>& eco_trait_alleles,
      const std::vector<double>& fem_pref_alleles,
      const std::vector<double>& male_trait_alleles
    );

    ///Create an individual fuzzily
    individual(const parameters& parameters);

    void init(const parameters& parameters, std::mt19937& gen);

    // Make a new baby from mother m and father f
    void birth(const individual& m, const individual& f, const parameters& m_p, std::mt19937& gen);


    double get_eco_trait() const noexcept { return m_x; }
    double get_fem_pref() const noexcept { return m_p; }
    double get_male_trait() const noexcept { return m_q; }

    const std::vector<double>& get_eco_trait_vector() const noexcept { return m_X; }
    const std::vector<double>& get_fem_pref_vector() const noexcept { return m_P; }
    const std::vector<double>& get_male_trait_vector() const noexcept { return m_Q; }

    friend bool operator==(const individual& lhs, const individual& rhs) noexcept;
};

///Create one offspring.
individual create_offspring(
  const individual& father,
  const individual& mother,
  const parameters& parameters,
  std::mt19937& rng_engine
);

///Creates just an individual for testing purposes
individual create_test_individual();

bool operator==(const individual& lhs, const individual& rhs) noexcept;
bool operator!=(const individual& lhs, const individual& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const individual& i) noexcept;

} //~namespace kewe

#endif // KEWE_INDIVIDUAL_H
