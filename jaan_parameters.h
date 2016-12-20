#ifndef JAAN_PARAMETERS_H
#define JAAN_PARAMETERS_H

class Parameters
{
public:
    Parameters();
    void set_gEnd(int input);
    void set_popSize(int input);
    void set_nPrefGenes(int input);
    void set_nTrtGenes(int input);
    void set_traitCost(double input);
    void set_pOpt(double input);
    void set_tOpt(double input);
    void set_deltap(double input);
    void set_deltat(double input);
    void set_mu(double input);
    int get_gEnd();
    int get_popSize();
    int get_nPrefGenes();
    int get_nTrtGenes();
    double get_traitCost();
    double get_pOpt();
    double get_tOpt();
    double get_deltap();
    double get_deltat();
    double get_mu();
private:
    int gEnd;           // How many generations per simulation?
    int popSize;        // How many individuals in each generation?
    int nPrefGenes;     // Number of genes for Preference.
    int nTrtGenes;      // Number of genes for Trait.
    double traitCost;   // How dangerous it is to have an Trait.
    double pOpt;        // Optimal preference value for viability.
    double tOpt;        // Optimal trait value for viability.
    double deltap;      // Higher = Lower cost of choosiness.
    double deltat;      // Higher = Lower cost of trait. Should be deltat << deltap
    double mu;          // Mutation rate.
};

#endif // JAAN_PARAMETERS_H
