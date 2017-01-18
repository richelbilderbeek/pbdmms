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
    void mate_select(std::vector<Individual>& population,
                    Parameters& p,
                    std::mt19937& generator);
    int attraction(std::vector<Individual>& population,
                   Parameters& p,
                   std::mt19937& generator,
                   double mateScore);
    void set_female_viability(double input);
    void set_male_viability(double input);
    void set_chance_to_be_father(double input);
    void set_pref_genes(std::vector<double> input);
    void set_trt_genes(std::vector<double> input);
    void set_preference(double input);
    void set_trait(double input);
    void set_mate(int input);
    double get_female_viability();
    double get_male_viability();
    double get_chance_to_be_father();
    std::vector<double> get_pref_genes();
    std::vector<double> get_trt_genes();
    double get_preference();
    double get_trait();
    int get_mate();
private:
    void mutate(Parameters& p,
                std::mt19937& generator);
    void develop(Parameters& p);
    double female_viability;        // Viability of the Individual as a female.
    double male_viability;          // Viability of the Individual as a male.
    /* How atttractive a male is as a function of his viability
     * and the match of his trait to the mother's preference. */
    double chance_to_be_father;
    std::vector<double> pref_genes; // Vector of the genes that sum to preference.
    std::vector<double> trt_genes;  // Vector of the genes that sum to preference.
    double preference;              // Preference expressed as a single number.
    double trait;                   // Trait expressed as a single number, without fitness inc.
    int mate;                       // Holds the position in the vector of the chosen male.
};

#endif // INDIVIDUAL_H
