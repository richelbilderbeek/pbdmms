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
    preference(p.get_scale_preference()),
    quality(0),
    trait(p.get_scale_trait())
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
    preference *= mean(pref_genes);
    quality = std::accumulate(
            std::begin(qual_genes),
            std::end(qual_genes),
            0.0);
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
    const int n_trt_genes = static_cast<int>(p.get_n_trt_genes());
    for (int i = 0; i < n_trt_genes; ++i)
    {
        if (distribution(generator) < 0.5)
            trt_genes[i] = -1;
        else
            trt_genes[i] = 1;
    }
    trait *= mean(trt_genes);
    preference *= mean(pref_genes);
    quality = std::accumulate(
            std::begin(qual_genes),
            std::end(qual_genes),
            0.0);
}

// PRIVATE INDIVIDUAL CLASS FUNCTIONS
void Individual::mutate_pref(
        std::mt19937& generator,
        const double& scale_pref)
{
    std::uniform_int_distribution<int> gene_dist(0, pref_genes.size() - 1);
    int i = gene_dist(generator);
    if (pref_genes[i] == -1)
        pref_genes[i] = 1;
    else if (pref_genes[i] == 1)
        pref_genes[i] = -1;
    preference = scale_pref * mean(pref_genes);
}

void Individual::mutate_trt(
        std::mt19937& generator,
        const double& scale_trt)
{
    std::uniform_int_distribution<int> gene_dist(0, trt_genes.size() - 1);
    int i = gene_dist(generator);
    if (trt_genes[i] == -1)
        trt_genes[i] = 1;
    else if (trt_genes[i] == 1)
        trt_genes[i] = -1;
    trait = scale_trt * mean(trt_genes);
}

void Individual::mutate_qual_inc(std::mt19937& generator)
{
    int n_genes = qual_genes.size();
    std::vector<int> qual_weights(n_genes);
    for (int i = 0; i < n_genes; ++i)
    {
        qual_weights[i] = !qual_genes[i];
    }
    std::discrete_distribution<int> gene_dist(qual_weights.begin(), qual_weights.end());
    qual_genes[gene_dist(generator)] = 1;
    quality = std::accumulate(
            std::begin(qual_genes),
            std::end(qual_genes),
            0.0);
}

void Individual::mutate_qual_dec(std::mt19937& generator)
{
    std::discrete_distribution<int> gene_dist(qual_genes.begin(), qual_genes.end());
    qual_genes[gene_dist(generator)] = 0;
    quality = std::accumulate(
            std::begin(qual_genes),
            std::end(qual_genes),
            0.0);
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
