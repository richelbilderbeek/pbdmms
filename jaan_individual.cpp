#include <iostream>
#include <cassert>
#include <stdexcept>
#include <random>
#include "jaan_individual.h"

Individual::Individual(Parameters& p,
                       std::mt19937& generator) :
female_viability(0.0),
male_viability(0.0),
chance_to_be_father(0.0),
pref_genes(p.get_n_pref_genes()),
trt_genes(p.get_n_trt_genes()),
qual_genes(p.get_n_qual_genes())
{
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
    develop(p);
}

Individual::Individual(const Individual& mother,
                       const Individual& father,
                       Parameters& p,
                       std::mt19937& generator) :
female_viability(0.0),
male_viability(0.0),
chance_to_be_father(0.0),
pref_genes(p.get_n_pref_genes()),
trt_genes(p.get_n_trt_genes()),
qual_genes(p.get_n_qual_genes())
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
    develop(p);
}

// CLASS FUNCTIONS
/* Function for Individuals to find a partner.
 * Chooses a mate by drawing a random number from a distribution created
 * by the cumulative size of the focal individual's preference and trait.
 */
int Individual::male_viability_function(std::vector<Individual>& population,
                            Parameters& p,
                            std::mt19937& generator)
{
    const int pop_size{static_cast<int>(p.get_pop_size())};
    for (int t = 0; t < pop_size; ++t)
        population[t].chance_to_be_father = population[t].male_viability *
                exp(get_preference() * population[t].get_trait() * population[t].get_quality());
    double mate_score = 0.0;
    for (int i = 0; i < pop_size; ++i) {
        mate_score += population[i].chance_to_be_father;
        population[i].chance_to_be_father = mate_score;
    }
    return pick_father(population, p, generator, mate_score);
}

int Individual::pick_father(std::vector<Individual>& population,
                           Parameters& p,
                           std::mt19937& generator,
                           double mate_score) {
    std::uniform_real_distribution<double> distribution(0.0, mate_score);
    const double choice = distribution(generator);

    const int pop_size{static_cast<int>(p.get_pop_size())};
    if (choice > population[pop_size - 1].chance_to_be_father)
        throw std::invalid_argument(
                "Choice is larger than highest chance_to_be_father value of population.");
    for (int i = 0; i < pop_size; ++i) {
        if (i != 0)
            assert(population[i].chance_to_be_father >= population[i-1].chance_to_be_father);
        if (population[i].chance_to_be_father >= choice)
            return i;
    }
    assert(!"Should never get here"); //!OCLINT accepted idiom, see Meyers Effective C++
    throw std::logic_error("Should never get here");
}

void Individual::set_female_viability(double input) {
    female_viability = input;
}

double Individual::get_female_viability() const noexcept {
    return female_viability;
}

double Individual::get_male_viability() const noexcept {
    return male_viability;
}

double Individual::get_chance_to_be_father() const noexcept {
    return chance_to_be_father;
}

double Individual::get_preference() const noexcept {
    double sum = 0;
    for (int i = 0; i < static_cast<int>(pref_genes.size()) - 1; ++i) {
        sum += pref_genes[i];
    }
    return sum /= static_cast<double>(pref_genes.size());
}

double Individual::get_trait() const noexcept {
    double sum = 0;
    for (int i = 0; i < static_cast<int>(trt_genes.size()) - 1; ++i) {
        sum += trt_genes[i];
    }
    return sum /= static_cast<double>(trt_genes.size());
}

double Individual::get_quality() const noexcept {
    double sum = 0;
    for (int i = 0; i < static_cast<int>(qual_genes.size()) - 1; ++i) {
        sum += qual_genes[i];
    }
    return sum /= static_cast<double>(qual_genes.size());
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

//	Calculate trait, preference and quality from the gene vectors.
void Individual::develop(Parameters& p)
{
    double temp = (get_preference() - p.get_optimal_preference()) / p.get_value_of_preference();
    female_viability = exp(-0.5 * temp * temp);

    temp = (get_trait() - p.get_optimal_trait()) / p.get_value_of_trait();
    male_viability = exp(-0.5 * temp * temp);
}

// OVERLOADED COMPARATOR
bool operator==(const Individual& lhs, const Individual& rhs) noexcept {
    return lhs.get_preference() == rhs.get_preference()
        && lhs.get_trait() == rhs.get_trait()
        && lhs.get_quality() == rhs.get_quality()
        && lhs.get_female_viability() == rhs.get_female_viability()
        && lhs.get_male_viability() == rhs.get_male_viability()
        && lhs.get_chance_to_be_father() == rhs.get_chance_to_be_father();
}
