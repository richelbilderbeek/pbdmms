#include <random>
#include "jaan_individual.h"

Individual::Individual(const jaan_parameters& p,
                       std::mt19937& generator) :
// Initialisation constructor.
vFemale(0.0),
vFcum(0.0),
vMale(0.0),
vMcum(0.0),
prefGenes(p.nPrefGenes),
trtGenes(p.nTrtGenes),
preference(0.0),
trait(0.0),
mate(-1)
{
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    for (int i = 0; i < p.nTrtGenes; ++i) { // Randomly set each gene to 0 or 1.
        if (distribution(generator) < 0.5) {
            trtGenes[i] = -1;
        }
        else {
            trtGenes[i] = 1;
        }
    }
    for (int i = 0; i < p.nPrefGenes; ++i) { // Randomly set each Preference gene to 0 or 1.
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
                       const jaan_parameters& p,
                       std::mt19937& generator) :
// Reproduction constructor.
vFemale(0.0),
vFcum(0.0),
vMale(0.0),
vMcum(0.0),
prefGenes(p.nPrefGenes),
trtGenes(p.nTrtGenes),
preference(0.0),
trait(0.0),
mate(-1)
{
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    // Inherit mother and father genes randomly 50:50.
    for (int i = 0; i < p.nTrtGenes; ++i) {
        if (distribution(generator) < 0.5) {
            trtGenes[i] = mother.trtGenes[i];
        }
        else {
            trtGenes[i] = father.trtGenes[i];
        }
    }
    // Inherit mother and father preference genes randomly 50:50.
    for (int i = 0; i < p.nPrefGenes; ++i) {
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

// CLASS FUNCTIONS
void Individual::mateSelect(
        const std::vector<Individual>& population,
        const jaan_parameters& p,
        std::mt19937& generator)
/*	Function for Individuals to find a partner.
 *  Selects randomly from the population vector.
 *  Function returns -1 if no suitable mate is found, otherwise returns the position of the mate.
 */
{
    // Sample the males at random.
    for (double t = 0.0; t < p.popSize; ++t) {
//        double mateScore = exp(preference * population[t].trait);
        std::uniform_int_distribution<int> pickMan(0, p.popSize - 1);
        int focal = pickMan(generator);
        if (population[focal].trait >= preference) {
            mate = focal; // If the male is good, return his position in vector.
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
void Individual::mutate(const jaan_parameters& p,
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
    for (int i = 0; i < p.nTrtGenes; ++i) {
        if (distribution(generator) < p.mu)  {
            if (trtGenes[i] == 1) {
                trtGenes[i] = -1;
            }
            else trtGenes[i] = 1;
        }
    }
    for (int i = 0; i < p.nPrefGenes; ++i) {
        if (distribution(generator) < p.mu) {
            if (prefGenes[i] == 1) {
                prefGenes[i] = -1;
            }
            else prefGenes[i] = 1;
        }
    }
}

void Individual::develop(const jaan_parameters& p)
/*	Calculate preference from prefGenes.
    Calculate trait from trtGenes.*/
{
    for (int i = 0; i < p.nPrefGenes; ++i) {
        preference += prefGenes[i];
    }
    preference /= p.nPrefGenes;
    double temp = (preference - p.pOpt) / p.deltap;
    vFemale = exp(-0.5 * temp * temp);

    for (int i = 0; i < p.nTrtGenes; ++i) {
        trait += trtGenes[i];
    }
    trait /= p.nTrtGenes;
    temp = (trait - p.tOpt) / p.deltat;
    vMale = exp(-0.5 * temp * temp);
}
