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
#include <cassert>
#include <vector>
#include <string>
#include <random>
#include "kewe_individual.h"
#include "kewe_parameters.h"
#include "kewe_results.h"

namespace kewe {

bool attractive_enough(
    const indiv& m,
    const indiv& f,
    const parameters& p,
    std::mt19937& gen
    );

bool fitness_high_enough(
    const indiv& i,
    const double comp_i,
    const indiv& j,
    const double comp_j,
    const parameters& parameters,
    std::mt19937& gen
    );

void create_header(const parameters& parameters);

double gauss(double xx, double sigma);

/// Pick random individual
bigint randomindividual(const std::vector<indiv>& pop, std::mt19937& gen);

double calc_competition(
    const unsigned int,
    const std::vector<indiv>& pop,
    const parameters& p
    );

double calc_survivability_indiv(const indiv& m, const double comp, const parameters& p);

double calc_attractiveness(
    const indiv& mother,
    const indiv& father,
    const parameters& parameters
    );

std::vector<indiv> create_initial_population(const parameters& parameters, std::mt19937& gen);

std::vector<indiv> create_next_generation(
  const parameters& parameters,
  const std::vector<indiv>& pop,
  std::mt19937& gen
);

unsigned int pick_individual(
    const std::vector<double>& pop_surv,
    const double surv,
    std::mt19937& gen
    );

void calc_pop_comp(
    const std::vector<indiv>& pop,
    const parameters& parameters,
    std::vector<double>& pop_comp
    );

double calc_and_set_survivability(
    const std::vector<indiv>& pop,
    const std::vector<double>& pop_comp,
    const parameters& parameters,
    std::vector<double>& pop_surv
    );

} //~namespace kewe

#endif // KEWE_SES_H
