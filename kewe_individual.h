#ifndef KEWE_INDIVIDUAL_H
#define KEWE_INDIVIDUAL_H

#include <vector>
#include <random>
#include "kewe_parameters.h"

class indiv
{
  private:
    std::vector<double>X;
    std::vector<double>P;
    std::vector<double>Q;
    double x,p,q;

    double a; // attractiveness

    void birth_haploid(const indiv& m, const indiv& f, const kewe_parameters& parameters);
    void birth_diploid(const indiv& m, const indiv& f, const kewe_parameters& parameters);

    void birth_haploid_trait(
        const double i,
        std::vector<double>& trait,
        double& avg_trait,
        const std::vector<double>& m_trait,
        const std::vector<double>& f_trait,
        const kewe_parameters& parameters
        );

    void birth_diploid_trait(
      const double i,
      std::vector<double>& trait,
      double& avg_trait,
      const std::vector<double>& m_trait,
      const std::vector<double>& f_trait,
      const kewe_parameters& parameters
  );

  public:
    indiv(const kewe_parameters& parameters);

    ///?
    void init(const kewe_parameters& parameters);

    // Make a new baby from mother m and father f
    void birth(const indiv& m, const indiv& f, const kewe_parameters& p);

    ///Suggest: use operator<< instead
    void print();

    ///What does this do?
    ///Suggest rename: get_[ecological_trait|male_trait|female_preference]
    double _x() const noexcept { return x;}

    ///What does this do?
    ///Suggest rename: get_[ecological_trait|male_trait|female_preference]
    double _p() const noexcept { return p;}

    ///What does this do?
    ///Suggest rename: get_[ecological_trait|male_trait|female_preference]
    double _q() const noexcept { return q;}

    ///What does this do?
    ///Suggest rename: get_[ecological_trait|male_trait|female_preference]
    double _a() const noexcept { return a;}

    ///What does this do?
    ///Suggest rename: get_[ecological_trait|male_trait|female_preference]
    const std::vector<double>& getX() const noexcept { return X; }

    ///What does this do?
    ///Suggest rename: get_[ecological_trait|male_trait|female_preference]
    const std::vector<double>& getP() const noexcept { return P; }

    ///What does this do?
    ///Suggest rename: get_[ecological_trait|male_trait|female_preference]
    const std::vector<double>& getQ() const noexcept { return Q; }

    ///What does this do?
    ///Suggest rename: set_[ecological_trait|male_trait|female_preference]
    void a_(double A) { a=A; return;}

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

#endif // KEWE_INDIVIDUAL_H
