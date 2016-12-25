#ifndef SADO_SIMULATION_H
#define SADO_SIMULATION_H

#include "sado_individual.h"

namespace sado {

int do_simulation(int argc, char *argv[]);

my_iterator end();

double gauss(double xx, double sigma);

void initialize();

void iterate();

void output(bigint t);

my_iterator randomindividual();

void readparameters(char *filename);

my_iterator start();



} //~namespace sado

#endif // SADO_SIMULATION_H
