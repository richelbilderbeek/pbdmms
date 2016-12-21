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
    const individual& m,
    const individual& f,
    const parameters& p,
    std::mt19937& gen
    );

bool fitness_high_enough(
    const individual& i,
    const double comp_i,
    const individual& j,
    const double comp_j,
    const parameters& parameters,
    std::mt19937& gen
    );

void create_header(const parameters& parameters);

/// Pick random individual
bigint randomindividual(const individuals& pop, std::mt19937& gen);

///Calculate the competition felt by an individual with ecological trait value a,
///with an individual with ecological trait value b, for a competition
///strength of sc
double calc_competition(
  const double a,
  const double b,
  const double sc
);

///Calculate the competition felt by individual at the
///population's index i
double calc_competition(
  const unsigned int i,
  const individuals& pop,
  const parameters& p
);

///Calculate the mortality of the individual
///See Van Doorn & Weissing 2002 equation [3]
///  @param eco_trait the phenotypical ecological_trait
///  @param eco_distr_width ecological distribution width
///  @param comp_intensity the competition intensity suffered by this individual
///  @param pop_size the number of individuals
double calc_mortality(
  const double ecological_trait,
  const double eco_distr_width,
  const double comp_intensity,
  const int pop_size
);


///Calculate the survivability of the individual, equals '1 - calc_mortality'
///  @param eco_distr_width ecological distribution width
///  @param comp_intensity the competition intensity suffered by this individual
double calc_survivability(
  const double ecological_trait,
  const double eco_distr_width,
  const double comp_intensity, //competition_intensity
  const int population_size
);

///Calculate the survibility of the individual, which is
///based on the ecological trait of the individual and the
///amount of competition it suffers
///
double calc_survivability(const individual& m, const double comp, const parameters& p);


individuals create_initial_population(const parameters& parameters, std::mt19937& gen);

individuals create_next_generation(
  const parameters& parameters,
  const individuals& pop,
  std::mt19937& gen
);

unsigned int pick_individual(
    const std::vector<double>& pop_surv,
    const double surv,
    std::mt19937& gen
    );

void calc_pop_comp(
    const individuals& pop,
    const parameters& parameters,
    std::vector<double>& pop_comp
    );

double calc_and_set_survivability(
    const individuals& pop,
    const std::vector<double>& pop_comp,
    const parameters& parameters,
    std::vector<double>& pop_surv
    );



} //~namespace kewe

#endif // KEWE_SES_H
