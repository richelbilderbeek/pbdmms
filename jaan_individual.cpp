#include <random>
#include "jaan_individual.h"

Individual::Individual(Parameters& p,
                       std::mt19937& generator) :
vFemale(0.0),
vFcum(0.0),
vMale(0.0),
vMcum(0.0),
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
vFcum(0.0),
vMale(0.0),
vMcum(0.0),
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
            && vFcum == rhs.vFcum
            && vMale == rhs.vMale
            && vMcum == rhs.vMcum;
}

// CLASS FUNCTIONS
void Individual::mateSelect(
        std::vector<Individual>& population,
        Parameters& p,
        std::mt19937& generator)
/* Function for Individuals to find a partner.
 * Chooses a mate by drawing a random number from a distribution created
 * by the cumulative size of the focal individual's preference and trait.
 */
{
    for (int t = 0; t < p.get_popSize(); ++t) {
        population[t].vMcum = exp(preference * population[t].trait);
    }
    double mateScore = 0.0;
    for (int i = 0; i < p.get_popSize(); ++i) {
        mateScore += population[i].vMcum;
        population[i].vMcum = mateScore;
    }
    std::uniform_real_distribution<double> distribution(0.0, mateScore);
    double choice = distribution(generator);
    for (int i = 0; i < p.get_popSize(); ++i) {
        if (population[0].vMcum < choice) {
            mate = 0;
        }
        else if ((i == p.get_popSize() - 1) & (population[i].vMcum < choice)) {
            mate = p.get_popSize();
        }
        else if ((population[i].vMcum < choice) & (population[i+1].vMcum > choice)) {
            mate = i + 1;
        }
    }
}

int Individual::getMate()
{
    return mate;
}

double Individual::getPref()
{
    return preference;
}

double Individual::getTrt()
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
