#ifndef KEWE_INDIVIDUAL_H
#define KEWE_INDIVIDUAL_H

#include <vector>
#include "kewe_parameters.h"

class indiv
{
    private:
    double x,p,q;
    std::vector<double>X;
    std::vector<double>P;
    std::vector<double>Q;
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
    indiv(const indiv &y);

    ///?
    void init(const kewe_parameters& parameters);

    // Make a new baby from mother m and father f
    void birth(const indiv& m, const indiv& f, const kewe_parameters& p);


    void print();
    double _x() const noexcept { return x;}
    double _p() const noexcept { return p;}
    double _q() const noexcept { return q;}
    double _a() const noexcept { return a;}
    void a_(double A) { a=A; return;}

    friend bool operator==(const indiv& lhs, const indiv& rhs) noexcept;
};

bool operator==(const indiv& lhs, const indiv& rhs) noexcept;
bool operator!=(const indiv& lhs, const indiv& rhs) noexcept;


#endif // KEWE_INDIVIDUAL_H
