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
    void mutate_pref(std::mt19937& generator, const double& scale_pref);
    void mutate_trt(std::mt19937& generator, const double& scale_trt);
    void mutate_qual_inc(std::mt19937& generator);
    void mutate_qual_dec(std::mt19937& generator);
private:
    std::vector<double> pref_genes; // Vector of the genes that sum to preference.
    std::vector<double> qual_genes; // Vector of the genes that sum to quality.
    std::vector<double> trt_genes;  // Vector of the genes that sum to preference.
    double preference;              // Expression of the preference of females.
    double quality;                 // "good genes" that factor into male attractiveness.
    double trait;                   // Expression of the trait of males.
};

void inherit_genes(
        std::uniform_real_distribution<double>& distribution,
        std::mt19937& generator,
        const int& n_pref_genes,
        std::vector<double>& my_genes,
        const std::vector<double>& mother_genes,
        const std::vector<double>& father_genes);

bool operator==(
        const Individual& lhs,
        const Individual& rhs) noexcept;

double mean(const std::vector<double>& list);

#endif // INDIVIDUAL_H
