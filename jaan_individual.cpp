#include <iostream>
#include <cassert>
#include <stdexcept>
#include <random>
#include "jaan_individual.h"

Individual::Individual(Parameters &p) :
    pref_genes(p.get_n_pref_genes()),
    trt_genes(p.get_n_trt_genes()),
    qual_genes(p.get_n_qual_genes()),
    preference(0),
    trait(0)
{
    for (int i = 0; i < static_cast<int>(pref_genes.size()); ++i) {
        pref_genes[i] = 0;
    }
    for (int i = 0; i < static_cast<int>(trt_genes.size()); ++i) {
        trt_genes[i] = 0;
    }
    for (int i = 0; i < static_cast<int>(qual_genes.size()); ++i) {
        qual_genes[i] = 0;
    }
}

Individual::Individual(const Individual& mother,
                       const Individual& father,
                       Parameters& p,
                       std::mt19937& generator) :
    pref_genes(p.get_n_pref_genes()),
    trt_genes(p.get_n_trt_genes()),
    qual_genes(p.get_n_qual_genes()),
    preference(0),
    trait(0)
{
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    const int n_trt_genes{static_cast<int>(p.get_n_trt_genes())};
    for (int i = 0; i < n_trt_genes; ++i) {
        if (distribution(generator) < 0.5)
            trt_genes[i] = mother.trt_genes[i];
        else
            trt_genes[i] = father.trt_genes[i];
    }
    const int n_pref_genes{static_cast<int>(p.get_n_pref_genes())};
    for (int i = 0; i < n_pref_genes; ++i) {
        if (distribution(generator) < 0.5)
            pref_genes[i] = mother.pref_genes[i];
        else
            pref_genes[i] = father.pref_genes[i];
    }
    const int n_qual_genes{static_cast<int>(p.get_n_qual_genes())};
    for (int i = 0; i < n_qual_genes; ++i) {
        if (distribution(generator) < 0.5)
            qual_genes[i] = mother.qual_genes[i];
        else
            qual_genes[i] = father.qual_genes[i];
    }
    const double pref_and_trt_mu{static_cast<double>(p.get_pref_and_trt_mu())};
    const double quality_inc_mu{static_cast<double>(p.get_quality_inc_mu())};
    const double quality_dec_mu{static_cast<double>(p.get_quality_dec_mu())};
    mutate(generator, distribution, n_trt_genes,
           trt_genes, pref_and_trt_mu, pref_and_trt_mu, 1, -1);
    mutate(generator, distribution, n_pref_genes,
           pref_genes, pref_and_trt_mu, pref_and_trt_mu, 1, -1);
    mutate(generator, distribution, n_qual_genes,
           qual_genes, quality_inc_mu, quality_dec_mu, 1, 0);
    trait = mean(trt_genes);
    preference = mean(pref_genes);
}

// CLASS FUNCTIONS
double Individual::get_preference() const noexcept {
    return preference;
}

double Individual::get_trait() const noexcept {
    return trait;
}

double Individual::get_quality() const noexcept {
    return mean(qual_genes);
}

void Individual::init_population(Parameters& p,
                                 std::mt19937& generator) {
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    const int n_trt_genes{static_cast<int>(p.get_n_trt_genes())};
    for (int i = 0; i < n_trt_genes; ++i) {
        if (distribution(generator) < 0.5)
            trt_genes[i] = -1;
        else
            trt_genes[i] = 1;
    }
    const int n_pref_genes{static_cast<int>(p.get_n_pref_genes())};
    for (int i = 0; i < n_pref_genes; ++i) {
        if (distribution(generator) < 0.5)
            pref_genes[i] = -1;
        else
            pref_genes[i] = 1;
    }
    const int n_qual_genes{static_cast<int>(p.get_n_qual_genes())};
    for (int i = 0; i < n_qual_genes; ++i) {
        if (distribution(generator) < 0.5)
            qual_genes[i] = -1;
        else
            qual_genes[i] = 1;
    }
    const double pref_and_trt_mu{static_cast<double>(p.get_pref_and_trt_mu())};
    const double quality_inc_mu{static_cast<double>(p.get_quality_inc_mu())};
    const double quality_dec_mu{static_cast<double>(p.get_quality_dec_mu())};
    mutate(generator, distribution, n_trt_genes,
           trt_genes, pref_and_trt_mu, pref_and_trt_mu, 1, -1);
    mutate(generator, distribution, n_pref_genes,
           pref_genes, pref_and_trt_mu, pref_and_trt_mu, 1, -1);
    mutate(generator, distribution, n_qual_genes,
           qual_genes, quality_inc_mu, quality_dec_mu, 1, 0);
    trait = mean(pref_genes);
    preference = mean(pref_genes);
}

// PRIVATE INDIVIDUAL CLASS FUNCTIONS
/* Takes a gene set, two mutation values and two possible
 * alleles and gives the genes a chance of flipping.
 */
void Individual::mutate(std::mt19937& generator,
                        std::uniform_real_distribution<double> distribution,
                        const int& n_genes,
                        std::vector<double>& gene_vector,
                        const double& mutation_rate_1,
                        const double& mutation_rate_2,
                        const double& gene_value_1,
                        const double& gene_value_2)
{
    for (int i = 0; i < n_genes; ++i) {
        const double probability = distribution(generator);
        if (gene_vector[i] == gene_value_2 && probability < mutation_rate_1)
            gene_vector[i] = gene_value_1;
        else if (gene_vector[i] == gene_value_1 && probability < mutation_rate_2)
            gene_vector[i] = gene_value_2;
    }
}

// OVERLOADED COMPARATOR
bool operator==(const Individual& lhs, const Individual& rhs) noexcept {
    return lhs.get_preference() == rhs.get_preference()
        && lhs.get_trait() == rhs.get_trait()
        && lhs.get_quality() == rhs.get_quality();
}

double mean(const std::vector<double>& list) {
    double sum = 0;
    for (int i = 0; i < static_cast<int>(list.size()); ++i) {
        sum += list[i];
    }
    sum /= static_cast<double>(list.size());
    return sum;
}
