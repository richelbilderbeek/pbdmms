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
preference(0.0),
trait(0.0),
mate(-1)
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
    mutate(p, generator);
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
preference(0.0),
trait(0.0),
mate(-1)
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
    mutate(p, generator);
    develop(p);
}

// OVERLOADED COMPARATOR
bool Individual::operator==(const Individual& rhs) const {
    return pref_genes == rhs.pref_genes
            && trt_genes == rhs.trt_genes
            && preference == rhs.preference
            && trait == rhs.trait
            && mate == rhs.mate
            && female_viability == rhs.female_viability
            && male_viability == rhs.male_viability
            && chance_to_be_father == rhs.chance_to_be_father;
}

// CLASS FUNCTIONS
void Individual::mate_select(std::vector<Individual>& population,
                            Parameters& p,
                            std::mt19937& generator)
/* Function for Individuals to find a partner.
 * Chooses a mate by drawing a random number from a distribution created
 * by the cumulative size of the focal individual's preference and trait.
 * ======================
 * male_viability is actually choosiness of the female.
 * ======================
 */
{
    const int pop_size{static_cast<int>(p.get_pop_size())};
    for (int t = 0; t < pop_size; ++t)
        population[t].chance_to_be_father = population[t].male_viability *
                exp(preference * population[t].trait);
    double mate_score = 0.0;
    for (int i = 0; i < pop_size; ++i) {
        mate_score += population[i].chance_to_be_father;
        population[i].chance_to_be_father = mate_score;
    }
    mate = mate_selection(population, p, generator, mate_score);
    if (mate < 0 || mate > pop_size)
        throw std::invalid_argument("mate_select function did not choose a father.");
}

int Individual::mate_selection(std::vector<Individual>& population,
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
            assert(population[i].chance_to_be_father > population[i-1].chance_to_be_father);
        if (population[i].chance_to_be_father >= choice)
            return i;
    }
    assert(!"Should never get here"); //!OCLINT accepted idiom, see Meyers Effective C++
    throw std::logic_error("Should never get here");
}

void Individual::set_female_viability(double input) {
    female_viability = input;
}

void Individual::set_male_viability(double input) {
    male_viability = input;
}

void Individual::set_chance_to_be_father(double input) {
    chance_to_be_father = input;
}

void Individual::set_pref_genes(std::vector<double> input) {
    pref_genes = input;
}

void Individual::set_trt_genes(std::vector<double> input) {
    trt_genes = input;
}

void Individual::set_preference(double input) {
    preference = input;
}

void Individual::set_trait(double input) {
    trait = input;
}

void Individual::set_mate(int input) {
    mate = input;
}

double Individual::get_female_viability() {
    return female_viability;
}

double Individual::get_male_viability() {
    return male_viability;
}

double Individual::get_chance_to_be_father() {
    return chance_to_be_father;
}

std::vector<double> Individual::get_pref_genes() {
    return pref_genes;
}

std::vector<double> Individual::get_trt_genes() {
    return trt_genes;
}

int Individual::get_mate()
{
    return mate;
}

double Individual::get_preference()
{
    return preference;
}

double Individual::get_trait()
{
    return trait;
}

//PRIVATE INDIVIDUAL CLASS FUNCTIONS
void Individual::mutate(Parameters& p,
                        std::mt19937& generator)
// Give each gene a chance of flipping.
/* =========================
 *
 * This section neads to favour deleterious mutations in the good gene.
 *
 * =========================
 */

{
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    const int n_trt_genes{static_cast<int>(p.get_n_trt_genes())};
    const int n_pref_genes{static_cast<int>(p.get_n_pref_genes())};
    const double mu{static_cast<double>(p.get_mu())};
    for (int i = 0; i < n_trt_genes; ++i) {
        if (distribution(generator) < mu)  {
            if (trt_genes[i] == 1)
                trt_genes[i] = -1;
            else
                trt_genes[i] = 1;
        }
    }
    for (int i = 0; i < n_pref_genes; ++i) {
        if (distribution(generator) < mu) {
            if (pref_genes[i] == 1)
                pref_genes[i] = -1;
            else
                pref_genes[i] = 1;
        }
    }
}

void Individual::develop(Parameters& p)
/*	Calculate preference from pref_genes.
    Calculate trait from trt_genes.*/
{
    const int n_pref_genes{static_cast<int>(p.get_n_pref_genes())};
    for (int i = 0; i < n_pref_genes; ++i)
        preference += pref_genes[i];
    preference /= n_pref_genes;
    double temp = (preference - p.get_optimal_preference()) / p.get_cost_of_preference();
    female_viability = exp(-0.5 * temp * temp);

    const int n_trt_genes{static_cast<int>(p.get_n_trt_genes())};
    for (int i = 0; i < n_trt_genes; ++i)
        trait += trt_genes[i];
    trait /= n_trt_genes;
    temp = (trait - p.get_optimal_trait()) / p.get_cost_of_trait();
    male_viability = exp(-0.5 * temp * temp);
}
