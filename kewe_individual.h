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

    public:
    indiv(const kewe_parameters& parameters);
    indiv(const indiv &y);

    ///?
    void init(const kewe_parameters& parameters);

    // Make a new baby from male m and female f
    void birth(indiv m, indiv f, const kewe_parameters& p);


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
