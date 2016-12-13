#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H
#include "jaan_parameters.h"

class Individual
{
public:
    Individual(const jaan_parameters& p,
               std::mt19937& generator);
    Individual(const Individual&,
               const Individual&,
               const jaan_parameters& p,
               std::mt19937& generator);
    void mateSelect(std::vector<Individual>& population,
                    const jaan_parameters& p,
                    std::mt19937& generator);
    double getPref();
    double getTrt();
    int getMate();
    double vFemale;
    double vFcum;
    double vMale;
    double vMcum;
private:
    void mutate(const jaan_parameters& p,
                std::mt19937& generator);
    void develop(const jaan_parameters& p);
    std::vector<double> prefGenes;
    std::vector<double> trtGenes;
    double preference;
    double trait;
    int mate;
};

#endif // INDIVIDUAL_H
