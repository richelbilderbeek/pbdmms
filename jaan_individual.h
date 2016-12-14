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
    void set_vFemale(double input);
    void set_vMale(double input);
    void set_prefGenes(std::vector<double> input);
    void set_trtGenes(std::vector<double> input);
    void set_Pref(double input);
    void set_Trt(double input);
    void set_Mate(int input);
    double get_vFemale();
    double get_vMale();
    std::vector<double> get_prefGenes();
    std::vector<double> get_trtGenes();
    double get_Pref();
    double get_Trt();
    int get_Mate();
private:
    void mutate(Parameters& p,
                std::mt19937& generator);
    void develop(Parameters& p);
    double vFemale;
    double vMale;
    std::vector<double> prefGenes;
    std::vector<double> trtGenes;
    double preference;
    double trait;
    int mate;
};

#endif // INDIVIDUAL_H
