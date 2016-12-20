#ifndef KEWE_INDIVIDUAL_H
#define KEWE_INDIVIDUAL_H

#include <vector>
#include <random>
#include "kewe_parameters.h"

namespace kewe {

class indiv
{
  private:
    std::vector<double>X;
    std::vector<double>P;
    std::vector<double>Q;
    double x,p,q;

    double a; // attractiveness
    double m_comp;

    void birth_haploid(
        const indiv& m,
        const indiv& f,
        const kewe_parameters& parameters,
        std::mt19937& gen
        );

    void birth_diploid(
        const indiv& m,
        const indiv& f,
        const kewe_parameters& parameters,
        std::mt19937& gen
        );

    void birth_haploid_trait(
        const double i,
        std::vector<double>& trait,
        double& avg_trait,
        const std::vector<double>& m_trait,
        const std::vector<double>& f_trait,
        const kewe_parameters& parameters,
        std::mt19937& gen
        );

    void birth_diploid_trait(
      const double i,
      std::vector<double>& trait,
      double& avg_trait,
      const std::vector<double>& m_trait,
      const std::vector<double>& f_trait,
      const kewe_parameters& parameters,
      std::mt19937& gen
  );

  public:
    indiv(const kewe_parameters& parameters);

    void init(const kewe_parameters& parameters, std::mt19937& gen);

    // Make a new baby from mother m and father f
    void birth(const indiv& m, const indiv& f, const kewe_parameters& p, std::mt19937& gen);


    double get_eco_trait() const noexcept { return x;}
    double get_fem_pref() const noexcept { return p;}
    double get_male_trait() const noexcept { return q;}
    double get_comp() const noexcept {return m_comp;}

    const std::vector<double>& get_eco_trait_vector() const noexcept { return X; }
    const std::vector<double>& get_fem_pref_vector() const noexcept { return P; }
    const std::vector<double>& get_male_trait_vector() const noexcept { return Q; }

    void set_comp(double comp) {m_comp = comp;}

    friend bool operator==(const indiv& lhs, const indiv& rhs) noexcept;
};

///Create one offspring.
indiv create_offspring(
    const indiv& father,
    const indiv& mother,
    const kewe_parameters& parameters,
    std::mt19937& rng_engine
    );

///Creates just an individual for testing purposes
indiv create_test_individual();

bool operator==(const indiv& lhs, const indiv& rhs) noexcept;
bool operator!=(const indiv& lhs, const indiv& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const indiv& i) noexcept;

} //~namespace kewe

#endif // KEWE_INDIVIDUAL_H
