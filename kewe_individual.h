#ifndef KEWE_INDIVIDUAL_H
#define KEWE_INDIVIDUAL_H

#include <vector>

class indiv
{
    private:
    double x,p,q;
    std::vector<double>X;
    std::vector<double>P;
    std::vector<double>Q;
    double a; // attractiveness

    public:
    indiv();
    indiv(const indiv &y);

    ///?
    void init(double x0, double p0, double q0);

    // Make a new baby from male m and female f
    void birth(indiv m, indiv f);


    void print();
    double _x() { return x;}
    double _p() { return p;}
    double _q() { return q;}
    double _a() { return a;}
    void a_(double A) { a=A; return;}

    friend bool operator==(const indiv& lhs, const indiv& rhs) noexcept;
};

bool operator==(const indiv& lhs, const indiv& rhs) noexcept;
bool operator!=(const indiv& lhs, const indiv& rhs) noexcept;


#endif // KEWE_INDIVIDUAL_H
