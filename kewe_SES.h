#ifndef KEWE_SES_H
#define KEWE_SES_H


#include<iostream>
#include<fstream>
#include<iomanip>
#include<cmath>
#include<cstdlib>
#include<cstring>
#include<list>
#include <stdexcept>
#include <numeric>
#include <limits>
//#include"kewe_random.h"
#include <cassert>
#include <vector>
#include <string>
#include "kewe_individual.h"
#include "kewe_parameters.h"
#include "kewe_results.h"

void create_header(const kewe_parameters& parameters);

double gauss(double xx, double sigma);

/// Pick random individual
bigint randomindividual(const std::vector<indiv>& pop, std::mt19937& gen);

double calc_competition(
    const unsigned int,
    const std::vector<indiv>& pop,
    const kewe_parameters& p
    );

double calc_survivability(const indiv& m, const double comp, const kewe_parameters& p);

double calc_attractiveness(
    const double pref,
    const double trait,
    const kewe_parameters& parameters
    );

std::vector<indiv> create_initial_population(const kewe_parameters& parameters, std::mt19937& gen);

std::vector<indiv> create_next_generation(
  const kewe_parameters& parameters,
  const std::vector<indiv>& pop,
  std::mt19937& gen
);

#endif // KEWE_SES_H
