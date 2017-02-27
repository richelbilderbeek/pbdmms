#include <iostream>
#include <cassert>
#include <stdexcept>
#include <random>
#include <numeric>
#include "jaan_individual.h"

Individual::Individual(const Parameters &p) :
    pref_genes(p.get_n_pref_genes(), 0),
    qual_genes(p.get_n_qual_genes(), 0),
    trt_genes(p.get_n_trt_genes(), 0),
    preference(0),
    quality(0),
    trait(0)
{
}

Individual::Individual(
        std::mt19937& generator,
        const Parameters& p,
        const Individual& father,
        const Individual& mother) :
    pref_genes(p.get_n_pref_genes()),
    qual_genes(p.get_n_qual_genes()),
    trt_genes(p.get_n_trt_genes()),
    preference(p.get_scale_preference()),
    quality(0),
    trait(p.get_scale_trait())
{
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    const int n_pref_genes = static_cast<int>(p.get_n_pref_genes());
    const int n_qual_genes = static_cast<int>(p.get_n_qual_genes());
    const int n_trt_genes = static_cast<int>(p.get_n_trt_genes());
    inherit_genes(distribution, generator, n_pref_genes,
                  pref_genes, mother.pref_genes, father.pref_genes);
    inherit_genes(distribution, generator, n_qual_genes,
                  qual_genes, mother.qual_genes, father.qual_genes);
    inherit_genes(distribution, generator, n_trt_genes,
                  trt_genes, mother.trt_genes, father.trt_genes);
    const double pref_and_trt_mu = static_cast<double>(p.get_pref_and_trt_mu());
    const double quality_inc_mu = static_cast<double>(p.get_quality_inc_mu());
    const double quality_dec_mu = static_cast<double>(p.get_quality_dec_mu());
    mutate(distribution, generator, pref_genes,
           n_pref_genes, pref_and_trt_mu, pref_and_trt_mu, 1, -1);
    mutate(distribution, generator, qual_genes,
           n_qual_genes, quality_inc_mu, quality_dec_mu, 1, 0);
    mutate(distribution, generator, trt_genes,
           n_trt_genes, pref_and_trt_mu, pref_and_trt_mu, 1, -1);
    preference *= mean(pref_genes);
    quality = mean(qual_genes);
    trait *= mean(trt_genes);
}

// CLASS FUNCTIONS
std::vector<double> Individual::get_pref_genes()
{
    return pref_genes;
}

std::vector<double> Individual::get_trt_genes()
{
    return trt_genes;
}

double Individual::get_preference() const noexcept
{
    return preference;
}

double Individual::get_quality() const noexcept
{
    return quality;
}

double Individual::get_trait() const noexcept
{
    return trait;
}

void Individual::init_population(
        std::mt19937& generator,
        const Parameters& p)
{
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    const int n_trt_genes = static_cast<int>(p.get_n_trt_genes());
    for (int i = 0; i < n_trt_genes; ++i)
    {
        if (distribution(generator) < 0.5)
            trt_genes[i] = -1;
        else
            trt_genes[i] = 1;
    }
    const int n_pref_genes = static_cast<int>(p.get_n_pref_genes());
    for (int i = 0; i < n_pref_genes; ++i)
    {
        if (distribution(generator) < 0.5)
            pref_genes[i] = -1;
        else
            pref_genes[i] = 1;
    }
    const int n_qual_genes = static_cast<int>(p.get_n_qual_genes());
    for (int i = 0; i < n_qual_genes; ++i)
    {
        if (distribution(generator) < 0.5)
            qual_genes[i] = 0;
        else
            qual_genes[i] = 1;
    }
    const double pref_and_trt_mu = static_cast<double>(p.get_pref_and_trt_mu());
    const double quality_inc_mu = static_cast<double>(p.get_quality_inc_mu());
    const double quality_dec_mu = static_cast<double>(p.get_quality_dec_mu());
    mutate(distribution, generator, trt_genes,
           n_trt_genes, pref_and_trt_mu, pref_and_trt_mu, 1, -1);
    mutate(distribution, generator, pref_genes,
           n_pref_genes, pref_and_trt_mu, pref_and_trt_mu, 1, -1);
    mutate(distribution, generator, qual_genes,
           n_qual_genes, quality_inc_mu, quality_dec_mu, 1, 0);
    trait = p.get_scale_trait() * mean(trt_genes);
    preference = p.get_scale_preference() * mean(pref_genes);
    quality = mean(qual_genes);
}

// PRIVATE INDIVIDUAL CLASS FUNCTIONS
/* Takes a gene set, two mutation values and two possible
 * alleles and gives the genes a chance of flipping.
 */
void Individual::mutate(
        std::uniform_real_distribution<double>& distribution,
        std::mt19937& generator,
        std::vector<double>& gene_vector,
        const int& n_genes,
        const double& mutation_rate_1,
        const double& mutation_rate_2,
        const double& gene_value_1,
        const double& gene_value_2)
{
    for (int i = 0; i < n_genes; ++i)
    {
        const double probability = distribution(generator);
        if (gene_vector[i] == gene_value_2 &&
            probability < mutation_rate_1)
            gene_vector[i] = gene_value_1;
        else if (gene_vector[i] == gene_value_1 &&
                 probability < mutation_rate_2)
            gene_vector[i] = gene_value_2;
    }
}

void inherit_genes(
        std::uniform_real_distribution<double>& distribution,
        std::mt19937& generator,
        const int& n_genes,
        std::vector<double>& my_genes,
        const std::vector<double>& mother_genes,
        const std::vector<double>& father_genes)
{
    for (int i = 0; i < n_genes; ++i)
    {
        if (distribution(generator) < 0.5)
            my_genes[i] = mother_genes[i];
        else
            my_genes[i] = father_genes[i];
    }
}

// OVERLOADED COMPARATOR
bool operator==(
        const Individual& lhs,
        const Individual& rhs) noexcept
{
    return lhs.get_preference() == rhs.get_preference()
        && lhs.get_trait() == rhs.get_trait()
        && lhs.get_quality() == rhs.get_quality();
}

double mean(const std::vector<double>& v)
{
    return std::accumulate(
            std::begin(v),
            std::end(v),
            0.0) /
        static_cast<double>(v.size());
}
