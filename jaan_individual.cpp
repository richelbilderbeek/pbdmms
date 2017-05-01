#include <iostream>
#include <cassert>
#include <stdexcept>
#include <random>
#include <numeric>
#include "jaan_individual.h"

/// Intiial generation of individuals
Individual::Individual(const Parameters &p) :
    pref_genes(p.get_n_pref_genes(), 0),
    qual_genes(p.get_n_qual_genes(), 0),
    trt_genes(p.get_n_trt_genes(), 0),
    preference(p.get_scale_preference()),
    quality(p.get_scale_quality()),
    trait(p.get_scale_trait())
{
}

/// Creates an individual from two parent individuals.
Individual::Individual(
        std::mt19937& generator,
        const Parameters& p,
        const Individual& father,
        const Individual& mother) :
    pref_genes(p.get_n_pref_genes()),
    qual_genes(p.get_n_qual_genes()),
    trt_genes(p.get_n_trt_genes()),
    preference(p.get_scale_preference()),
    quality(p.get_scale_quality()),
    trait(p.get_scale_trait())
{
    /// Create a distribution to pick from to assign genes equally to two alleles.
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    const int n_pref_genes = static_cast<int>(p.get_n_pref_genes());
    const int n_qual_genes = static_cast<int>(p.get_n_qual_genes());
    const int n_trt_genes = static_cast<int>(p.get_n_trt_genes());
    /// Determine which genes are inherited from which parent for each gene sequence.
    inherit_genes(distribution, generator, n_pref_genes,
                  pref_genes, mother.pref_genes, father.pref_genes);
    inherit_genes(distribution, generator, n_qual_genes,
                  qual_genes, mother.qual_genes, father.qual_genes);
    inherit_genes(distribution, generator, n_trt_genes,
                  trt_genes, mother.trt_genes, father.trt_genes);
    /// Calculate the phenotypes of the individual for each sequence.
    preference *= mean(pref_genes);
    quality *= mean(qual_genes);
    trait *= mean(trt_genes);
}

// CLASS FUNCTIONS
std::vector<double> Individual::get_pref_genes() const noexcept
{
    return pref_genes;
}

std::vector<double> Individual::get_qual_genes() const noexcept
{
    return qual_genes;
}

std::vector<double> Individual::get_trt_genes() const noexcept
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

/// Randomises the population giving it large variation.
void Individual::init_genetics(std::mt19937& generator)
{
    /// Create a distribution to pick from to assign genes equally to two alleles.
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    const int n_pref_genes = static_cast<int>(pref_genes.size());
    /// Randomly assign one allele or the other to pref genes.
    for (int i = 0; i < n_pref_genes; ++i)
    {
        if (distribution(generator) > 0.5)
        {
            pref_genes[i] = -1;
        }
        else
        {
            pref_genes[i] = 1;
        }
    }
    const int n_qual_genes = static_cast<int>(qual_genes.size());
    /// Randomly assign one allele or the other to qual genes.
    for (int i = 0; i < n_qual_genes; ++i)
    {
        if (distribution(generator) > 0.5)
        {
            qual_genes[i] = -1;
        }
        else
        {
            qual_genes[i] = 1;
        }
    }
    const int n_trt_genes = static_cast<int>(trt_genes.size());
    /// Randomly assign one allele or the other to trt genes.
    for (int i = 0; i < n_trt_genes; ++i)
    {
        if (distribution(generator) > 0.5)
        {
            trt_genes[i] = -1;
        }
        else
        {
            trt_genes[i] = 1;
        }
    }
    /// Calculate the phenotypes of the individual for each sequence.
    preference *= mean(pref_genes);
    quality *= mean(qual_genes);
    trait *= mean(trt_genes);
}

/// PRIVATE INDIVIDUAL CLASS FUNCTIONS
/// Randomly choose a gene for preference to mutate and
/// switch from allele 1 to allele -1 or vice versa.
void Individual::mutate_pref(
        std::mt19937& generator,
        const double& scale_pref)
{
    std::uniform_int_distribution<int> gene_dist(0, pref_genes.size() - 1);
    const int i = gene_dist(generator);
    if (pref_genes[i] == -1)
    {
        pref_genes[i] = 1;
    }
    else
    {
        pref_genes[i] = -1;
    }
    /// Recalculate the preference.
    preference = scale_pref * mean(pref_genes);
}

/// Randomly choose a gene for trait to mutate and
/// switch from allele 1 to allele -1 or vice versa.
void Individual::mutate_trt(
        std::mt19937& generator,
        const double& scale_trt)
{
    std::uniform_int_distribution<int> gene_dist(0, trt_genes.size() - 1);
    const int i = gene_dist(generator);
    if (trt_genes[i] == -1)
    {
        trt_genes[i] = 1;
    }
    else
    {
        trt_genes[i] = -1;
    }
    /// Recalculate the trait.
    trait = scale_trt * mean(trt_genes);
}

/// Randomly choose a gene for trait to mutate and
/// switch from allele 1 to allele -1 or vice versa.
void Individual::mutate_qual(
        std::mt19937& generator,
        const double& scale_qual)
{
    std::uniform_int_distribution<int> gene_dist(0, qual_genes.size() - 1);
    const int i = gene_dist(generator);
    if (qual_genes[i] == -1)
    {
        qual_genes[i] = 1;
    }
    else
    {
        qual_genes[i] = -1;
    }
    /// Recalculate the trait.
    quality = scale_qual * mean(qual_genes);
}

/// Choose equally which parent to inherit each gene of a gene sequence from.
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
        {
            my_genes[i] = mother_genes[i];
        }
        else
        {
            my_genes[i] = father_genes[i];
        }
    }
}

/// OVERLOADED COMPARATOR
/// Check Individual lhs is the same as Individual rhs
bool operator==(
        const Individual& lhs,
        const Individual& rhs) noexcept
{
    return lhs.get_pref_genes() == rhs.get_pref_genes()
        && lhs.get_qual_genes() == rhs.get_qual_genes()
        && lhs.get_trt_genes() == rhs.get_trt_genes()
        && lhs.get_preference() == rhs.get_preference()
        && lhs.get_quality() == rhs.get_quality()
        && lhs.get_trait() == rhs.get_trait();
}

/// Helper function for calculating the mean.
double mean(const std::vector<double>& v)
{
    return std::accumulate(
            std::begin(v),
            std::end(v),
            0.0) /
        static_cast<double>(v.size());
}
