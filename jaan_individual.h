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
    Individual(const Parameters& p);
    Individual(const Individual&,
               const Individual&,
               const Parameters& p,
               std::mt19937& generator);
    double get_preference() const noexcept;
    double get_trait() const noexcept;
    double get_quality() const noexcept;
    void init_population(const Parameters& p,
                         std::mt19937& generator);
private:
    void mutate(std::mt19937& generator,
                std::uniform_real_distribution<double> distribution,
                const int& n_genes,
                std::vector<double>& gene_vector,
                const double& mutation_rate_1,
                const double& mutation_rate_2,
                const double& gene_value_1,
                const double& gene_value_2);
    std::vector<double> pref_genes; // Vector of the genes that sum to preference.
    std::vector<double> trt_genes;  // Vector of the genes that sum to preference.
    std::vector<double> qual_genes; // Vector of the genes that sum to quality.
    double preference;              // Expression of the preference of females.
    double trait;                   // Expression of the trait of males.
    double quality;                 // "good genes" that factor into male attractiveness.
};

bool operator==(const Individual& lhs, const Individual& rhs) noexcept;

double mean(const std::vector<double>& list);

#endif // INDIVIDUAL_H
