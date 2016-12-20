#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H
#include <random>
#include "jaan_parameters.h"

class Individual
{
public:
    Individual(Parameters& p,
               std::mt19937& generator);
    Individual(const Individual&,
               const Individual&,
               Parameters& p,
               std::mt19937& generator);
    bool operator==(const Individual& rhs) const;
    void mateSelect(std::vector<Individual>& population,
                    Parameters& p,
                    std::mt19937& generator);
    double getPref();
    double getTrt();
    int getMate();
    double vFemale;
    double vFcum;
    double vMale;
    double vMcum;
    std::vector<double> prefGenes;
    std::vector<double> trtGenes;
    double preference;
    double trait;
    int mate;
private:
    void mutate(Parameters& p,
                std::mt19937& generator);
    void develop(Parameters& p);
};

#endif // INDIVIDUAL_H
