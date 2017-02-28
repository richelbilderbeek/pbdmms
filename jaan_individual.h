#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H
#include <random>
#include "jaan_parameters.h"

class Individual
{
public:
    Individual(const Parameters& p);
    Individual(
            std::mt19937& generator,
            const Parameters& p,
            const Individual& mother,
            const Individual& father);
    std::vector<double> get_pref_genes();
    std::vector<double> get_trt_genes();
    double get_preference() const noexcept;
    double get_quality() const noexcept;
    double get_trait() const noexcept;
    void init_population(
            std::mt19937& generator,
            const Parameters& p);
    void mutate(
            std::uniform_real_distribution<double>& distribution,
            std::mt19937& generator,
            std::vector<double>& gene_vector,
            const int& n_genes,
            const double& mutation_rate_1,
            const double& mutation_rate_2,
            const double& gene_value_1,
            const double& gene_value_2);
private:
    std::vector<double> pref_genes; // Vector of the genes that sum to preference.
    std::vector<double> qual_genes; // Vector of the genes that sum to quality.
    std::vector<double> trt_genes;  // Vector of the genes that sum to preference.
    double preference;              // Expression of the preference of females.
    double quality;                 // "good genes" that factor into male attractiveness.
    double trait;                   // Expression of the trait of males.
};

bool operator==(
        const Individual& lhs,
        const Individual& rhs) noexcept;

void inherit_genes(
        std::uniform_real_distribution<double>& distribution,
        std::mt19937& generator,
        const int& n_pref_genes,
        std::vector<double>& my_genes,
        const std::vector<double>& mother_genes,
        const std::vector<double>& father_genes);

double mean(const std::vector<double>& list);

#endif // INDIVIDUAL_H
