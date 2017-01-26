#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H
#include <random>
#include "jaan_parameters.h"

/*
 * =====================================
 *
 * Change constructors to generate an identical individual and then they should randomise
 * afterwards. Make sure that the individuals once randomised follow the distribution they ought
 * to. Why are their traits negative and why are their preferences positive?
 *
 * =====================================
 */

class Individual
{
public:
    Individual(Parameters& p,
               std::mt19937& generator);
    Individual(const Individual&,
               const Individual&,
               Parameters& p,
               std::mt19937& generator);
    int male_viability_function(std::vector<Individual>& population,
                    Parameters& p,
                    std::mt19937& generator);
    int pick_father(std::vector<Individual>& population,
                   Parameters& p,
                   std::mt19937& generator,
                   double mate_score);
    void set_female_viability(double input);
    double get_female_viability() const noexcept;
    double get_male_viability() const noexcept;
    double get_chance_to_be_father()const noexcept;
    double get_preference() const noexcept;
    double get_trait() const noexcept;
    double get_quality() const noexcept;
private:
    void mutate(std::mt19937& generator,
                std::uniform_real_distribution<double> distribution,
                const int& n_genes,
                std::vector<double>& gene_vector,
                const double& mutation_rate_1,
                const double& mutation_rate_2,
                const double& gene_value_1,
                const double& gene_value_2);
    void develop(Parameters& p);
    double female_viability;        // Viability of the Individual as a female.
    double male_viability;          // Viability of the Individual as a male.
    /* How atttractive a male is as a function of his viability
     * and the match of his trait to the mother's preference. */
    double chance_to_be_father;
    std::vector<double> pref_genes; // Vector of the genes that sum to preference.
    std::vector<double> trt_genes;  // Vector of the genes that sum to preference.
    std::vector<double> qual_genes; // Vector of the genes that sum to quality.
    double preference;              // Mean of the pref_genes.
    double trait;                   // Mean of the trt_genes.
};

int pick_father(const Individual& mother,
                std::vector<Individual>& population,
                Parameters& p,
                std::mt19937& generator,
                double mate_score);

bool operator==(const Individual& lhs, const Individual& rhs) noexcept;

#endif // INDIVIDUAL_H
