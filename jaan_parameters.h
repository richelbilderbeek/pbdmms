#ifndef JAAN_PARAMETERS_H
#define JAAN_PARAMETERS_H

struct jaan_parameters
{
    int gEnd          = 1;                                         // How many generations?
    int popSize       = 1000;                                      // How many individuals in each generation?
    int nPrefGenes    = 10;                                        // Number of genes for Preference.
    int nTrtGenes     = 10;                                        // Number of genes for Trait.
    double traitCost  = 0.0;                                       // How dangerous it is to have an Trait.
    double pOpt       = 0.0;                                       // Optimal preference value for viability.
    double tOpt       = 0.0;                                       // Optimal trait value for viability.
    double deltap     = 1;                                         // Higher = Lower cost of choosiness.
    double deltat     = 0.09;                                      // Higher = Lower cost of trait. Should be deltat << deltap
    double mu         = 1.0e-4;                                    // Mutation rate.
};

#endif // JAAN_PARAMETERS_H
