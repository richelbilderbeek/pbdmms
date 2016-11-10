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
#include"kewe_random.h"
#include <cassert>
#include <vector>
#include <string>
#include "kewe_individual.h"
#include "kewe_parameters.h"
#include "kewe_results.h"

double gauss(double xx, double sigma);

/// Pick random individual
bigint randomindividual(const std::vector<indiv>& pop);

std::vector<indiv> initialize(void);

void iterate(
  std::vector<std::vector<double>> &histX,
  std::vector<std::vector<double>> &histP,
  std::vector<std::vector<double>> &histQ,
  const kewe_parameters& parameters,
  std::vector<indiv>& pop
);

#endif // KEWE_SES_H
