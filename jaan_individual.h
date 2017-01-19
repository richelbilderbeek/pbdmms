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
    int mate_selection(std::vector<Individual>& population,
                   Parameters& p,
                   std::mt19937& generator,
                   double mate_score);
    void set_female_viability(double input);
    void set_male_viability(double input);
    void set_chance_to_be_father(double input);
    void set_pref_genes(std::vector<double> input);
    void set_trt_genes(std::vector<double> input);
    void set_qual_genes(std::vector<double> input);
    void set_preference(double input);
    void set_trait(double input);
    void set_quality(double input);
    void set_mate(int input);
    double get_female_viability();
    double get_male_viability();
    double get_chance_to_be_father();
    std::vector<double> get_pref_genes();
    std::vector<double> get_trt_genes();
    std::vector<double> get_qual_genes();
    double get_preference();
    double get_trait();
    double get_quality();
    int get_mate();
private:
    void mutate(Parameters& p,
                std::mt19937& generator);
    void mutate_trait(std::mt19937& generator,
                      std::uniform_real_distribution<double> distribution,
                      const int n_trt_genes,
                      const double pref_and_trt_mu);
    void mutate_preference(std::mt19937& generator,
                           std::uniform_real_distribution<double> distribution,
                           const int n_pref_genes,
                           const double pref_and_trt_mu);
    void mutate_quality(std::mt19937& generator,
                        std::uniform_real_distribution<double> distribution,
                        const int n_qual_genes,
                        const double quality_inc_mu,
                        const double quality_dec_mu);
    void develop(Parameters& p);
    double female_viability;        // Viability of the Individual as a female.
    double male_viability;          // Viability of the Individual as a male.
    /* How atttractive a male is as a function of his viability
     * and the match of his trait to the mother's preference. */
    double chance_to_be_father;
    std::vector<double> pref_genes; // Vector of the genes that sum to preference.
    std::vector<double> trt_genes;  // Vector of the genes that sum to preference.
    std::vector<double> qual_genes; // Vector of the genes that sum to quality.
    double preference;              // Preference expressed as a single number.
    double trait;                   // Trait expressed as a single number, without fitness inc.
    double quality;                 // The adaptation of the individual to its habitat.
    int mate;                       // Holds the position in the vector of the chosen male.
};

#endif // INDIVIDUAL_H
