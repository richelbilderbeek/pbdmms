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
    void set_attract(double input);
    void set_prefGenes(std::vector<double> input);
    void set_trtGenes(std::vector<double> input);
    void set_Pref(double input);
    void set_Trt(double input);
    void set_Mate(int input);
    double get_vFemale();
    double get_vMale();
    double get_attract();
    std::vector<double> get_prefGenes();
    std::vector<double> get_trtGenes();
    double get_Pref();
    double get_Trt();
    int get_Mate();
private:
    void mutate(Parameters& p,
                std::mt19937& generator);
    void develop(Parameters& p);
    double vFemale;                 // Viability of the Individual as a female.
    double vMale;                   // Viability of the Individual as a male.
    /* How attractive a male is as a function of his viability
     * and the match of his trait to the mother's preference. */
    double attract;
    std::vector<double> prefGenes;  // Vector of the genes that sum to preference.
    std::vector<double> trtGenes;   // Vector of the genes that sum to preference.
    double preference;              // Preference expressed as a single number.
    double trait;                   // Trait expressed as a single number, without fitness inc.
    int mate;                       // Holds the position in the vector of the chosen male.
};

#endif // INDIVIDUAL_H
