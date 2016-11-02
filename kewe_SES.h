#ifndef KEWE_SES_H
#define KEWE_SES_H


#include <list>
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

class simulation
{
public:
  /// Default construction has testing parameters (FOR NOW)
  simulation();
  void run();
};

typedef std::list<indiv>::iterator my_iterator;
typedef std::list<indiv>::const_iterator my_const_iterator;

///Creates a parameter file that can be used for testing with the name 'filename'
///Will throw if file cannot be created
void create_test_parameter_file(const std::string& filename);

my_iterator start(void);

my_iterator end(void);

double gauss(double xx, double sigma);

/// Pick random individual
my_iterator randomindividual(std::list<indiv>& pop);

std::list<indiv> initialize(void);

void output(
  bigint t,
  std::vector<std::vector<double>> &histX,
  std::vector<std::vector<double>> &histP,
  std::vector<std::vector<double>> &histQ,
  const kewe_parameters& parameters);

void iterate(
  std::vector<std::vector<double>> &histX,
  std::vector<std::vector<double>> &histP,
  std::vector<std::vector<double>> &histQ,
  const kewe_parameters& parameters,
  std::list<indiv>& pop
);


kewe_parameters readparameters(const std::string& filename);

// Count number of borders (from 0 to >0 or from >0 to 0) in a histogram
int countBorders(const std::vector<double> &histogram);

// calculates lineages (borders / 2) and the trait with the most lineages becomes
// the number of lineages for that generation
int countLineagesForGen(const int t,
                        const std::vector<std::vector<double>> &histX,
                        const std::vector<std::vector<double>> &histP,
                        const std::vector<std::vector<double>> &histQ);
//output all number of lineages for all the generations
void outputLTT(const std::vector<std::vector<double>> &histX,
               const std::vector<std::vector<double>> &histP,
               const std::vector<std::vector<double>> &histQ,
               const kewe_parameters& parameters);

#endif // KEWE_SES_H
