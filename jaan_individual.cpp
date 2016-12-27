#include <iostream>
#include <stdexcept>
#include <random>
#include "jaan_individual.h"

Individual::Individual(Parameters& p,
                       std::mt19937& generator) :
vFemale(0.0),
vMale(0.0),
attract(0.0),
prefGenes(p.get_nPrefGenes()),
trtGenes(p.get_nTrtGenes()),
preference(0.0),
trait(0.0),
mate(-1)
{
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    for (int i = 0; i < p.get_nTrtGenes(); ++i) {
        if (distribution(generator) < 0.5) {
            trtGenes[i] = -1;
        }
        else {
            trtGenes[i] = 1;
        }
    }
    for (int i = 0; i < p.get_nPrefGenes(); ++i) {
        if (distribution(generator) < 0.5) {
            prefGenes[i] = -1;
        }
        else {
            prefGenes[i] = 1;
        }
    }
    mutate(p, generator);
    develop(p);
}

Individual::Individual(const Individual& mother,
                       const Individual& father,
                       Parameters& p,
                       std::mt19937& generator) :
vFemale(0.0),
vMale(0.0),
attract(0.0),
prefGenes(p.get_nPrefGenes()),
trtGenes(p.get_nTrtGenes()),
preference(0.0),
trait(0.0),
mate(-1)
{
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    for (int i = 0; i < p.get_nTrtGenes(); ++i) {
        if (distribution(generator) < 0.5) {
            trtGenes[i] = mother.trtGenes[i];
        }
        else {
            trtGenes[i] = father.trtGenes[i];
        }
    }
    for (int i = 0; i < p.get_nPrefGenes(); ++i) {
        if (distribution(generator) < 0.5) {
            prefGenes[i] = mother.prefGenes[i];
        }
        else {
            prefGenes[i] = father.prefGenes[i];
        }
    }
    mutate(p, generator);
    develop(p);
}

// OVERLOADED COMPARATOR
bool Individual::operator==(const Individual& rhs) const {
    return prefGenes == rhs.prefGenes
            && trtGenes == rhs.trtGenes
            && preference == rhs.preference
            && trait == rhs.trait
            && mate == rhs.mate
            && vFemale == rhs.vFemale
            && vMale == rhs.vMale
            && attract == rhs.attract;
}

// CLASS FUNCTIONS
void Individual::mateSelect(
        std::vector<Individual>& population,
        Parameters& p,
        std::mt19937& generator)
/* Function for Individuals to find a partner.
 * Chooses a mate by drawing a random number from a distribution created
 * by the cumulative size of the focal individual's preference and trait.
 * ======================
 * vMale is actually choosiness of the female.
 * ======================
 */
{
    for (int t = 0; t < p.get_popSize(); ++t) {
        population[t].attract = population[t].vMale * exp(preference * population[t].trait);
    }
    double mateScore = 0.0;
    for (int i = 0; i < p.get_popSize(); ++i) {
        mateScore += population[i].attract;
        population[i].attract = mateScore;
    }
    std::uniform_real_distribution<double> distribution(0.0, mateScore);
    double choice = distribution(generator);
    for (int i = 0; i < p.get_popSize(); ++i) {
        if (population[0].attract > choice) {
            mate = 0;
            break;
        }
        else if ((i == (p.get_popSize() - 1)) && (population[i].attract < choice)) {
            mate = p.get_popSize();
            break;
        }
        else if ((population[i].attract > choice) && (population[i-1].attract < choice)) {
            mate = i;
            break;
        }
    }
    if (mate < 0 || mate > p.get_popSize()) {
        throw std::invalid_argument("mateSelect function did not choose a father.");
    }
}

void Individual::set_vFemale(double input) {
    vFemale = input;
}

void Individual::set_vMale(double input) {
    vMale = input;
}

void Individual::set_attract(double input) {
    attract = input;
}

void Individual::set_prefGenes(std::vector<double> input) {
    prefGenes = input;
}

void Individual::set_trtGenes(std::vector<double> input) {
    trtGenes = input;
}

void Individual::set_Pref(double input) {
    preference = input;
}

void Individual::set_Trt(double input) {
    trait = input;
}

void Individual::set_Mate(int input) {
    mate = input;
}

double Individual::get_vFemale() {
    return vFemale;
}

double Individual::get_vMale() {
    return vMale;
}

double Individual::get_attract() {
    return attract;
}

std::vector<double> Individual::get_prefGenes() {
    return prefGenes;
}

std::vector<double> Individual::get_trtGenes() {
    return trtGenes;
}

int Individual::get_Mate()
{
    return mate;
}

double Individual::get_Pref()
{
    return preference;
}

double Individual::get_Trt()
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
    for (int i = 0; i < p.get_nTrtGenes(); ++i) {
        if (distribution(generator) < p.get_mu())  {
            if (trtGenes[i] == 1) {
                trtGenes[i] = -1;
            }
            else trtGenes[i] = 1;
        }
    }
    for (int i = 0; i < p.get_nPrefGenes(); ++i) {
        if (distribution(generator) < p.get_mu()) {
            if (prefGenes[i] == 1) {
                prefGenes[i] = -1;
            }
            else prefGenes[i] = 1;
        }
    }
}

void Individual::develop(Parameters& p)
/*	Calculate preference from prefGenes.
    Calculate trait from trtGenes.*/
{
    for (int i = 0; i < p.get_nPrefGenes(); ++i) {
        preference += prefGenes[i];
    }
    preference /= p.get_nPrefGenes();
    double temp = (preference - p.get_pOpt()) / p.get_deltap();
    vFemale = exp(-0.5 * temp * temp);

    for (int i = 0; i < p.get_nTrtGenes(); ++i) {
        trait += trtGenes[i];
    }
    trait /= p.get_nTrtGenes();
    temp = (trait - p.get_tOpt()) / p.get_deltat();
    vMale = exp(-0.5 * temp * temp);
}
