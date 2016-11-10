#ifndef KEWE_RESULTS_H
#define KEWE_RESULTS_H

#include <iostream>
#include <vector>
#include "kewe_parameters.h"
#include "kewe_individual.h"


///For visualization
using histograms_in_time = std::vector<std::vector<double>>;

struct results
{
  histograms_in_time m_ecological_trait;
  histograms_in_time m_female_preference;
  histograms_in_time m_male_trait;
};

struct genotypes
{
  double m_x;
  double m_p;
  double m_q;
};

genotypes calc_average_genotype(const std::vector<indiv>& pop);

void output(bigint t,
            std::vector<std::vector<double>> &histX,
            std::vector<std::vector<double>> &histP,
            std::vector<std::vector<double>> &histQ,
            const kewe_parameters& parameters,
            const std::vector<indiv>& pop
            );

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

///Creates a file with the 'golden' output file from Van Doorn
void recreate_golden_output(const std::string& filename);

results get_results();

#endif // KEWE_RESULTS_H
