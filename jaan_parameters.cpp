#include <stdexcept>
#include "jaan_parameters.h"

Parameters::Parameters() :
    gEnd(1000),     // How many generations?
    popSize(1000),  // How many individuals in each generation?
    nPrefGenes(10), // Number of genes for Preference.
    nTrtGenes(10),  // Number of genes for Trait.
    traitCost(0.0), // How dangerous it is to have an Trait.
    pOpt(0.0),      // Optimal preference value for viability.
    tOpt(0.0),      // Optimal trait value for viability.
    deltap(1),      // Higher = Lower cost of choosiness.
    deltat(0.09),   // Higher = Lower cost of trait. Should be deltat << deltap
    mu(1.0e-4)      // Mutation rate.
{
}

void Parameters::set_gEnd(int input) {
    if (input < 0) {
        throw std::invalid_argument("Final generation must be a positive integer.");
    }
    gEnd = input;
}

void Parameters::set_popSize(int input) {
    if (input < 0) {
        throw std::invalid_argument("Population size must be a positive integer.");
    }
    popSize = input;
}

void Parameters::set_nPrefGenes(int input) {
    if (input < 0) {
        throw std::invalid_argument("Number of preference genes must be a positive integer.");
    }
    nPrefGenes = input;
}

void Parameters::set_nTrtGenes(int input) {
    if (input < 0) {
        throw std::invalid_argument("Number of trait genes must be a positive integer.");
    }
    nTrtGenes = input;
}

void Parameters::set_traitCost(double input) {
    traitCost = input;
}

void Parameters::set_pOpt(double input) {
    pOpt = input;
}

void Parameters::set_tOpt(double input) {
    tOpt = input;
}

void Parameters::set_deltap(double input) {
    deltap = input;
}

void Parameters::set_deltat(double input) {
    deltat = input;
}

void Parameters::set_mu(double input) {
    mu = input;
}

int Parameters::get_gEnd() {
    return gEnd;
}

int Parameters::get_popSize() {
    return popSize;
}

int Parameters::get_nPrefGenes() {
    return nPrefGenes;
}

int Parameters::get_nTrtGenes() {
    return nTrtGenes;
}

double Parameters::get_traitCost() {
    return traitCost;
}

double Parameters::get_pOpt() {
    return pOpt;
}

double Parameters::get_tOpt() {
    return tOpt;
}

double Parameters::get_deltap() {
    return deltap;
}

double Parameters::get_deltat() {
    return deltat;
}

double Parameters::get_mu() {
    return mu;
}

