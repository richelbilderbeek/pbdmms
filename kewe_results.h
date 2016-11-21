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

struct result_variables
{
  std::vector<int> m_t;

  std::vector<double> m_popsize;
  std::vector<double> m_rhoxp;
  std::vector<double> m_rhoxq;
  std::vector<double> m_rhopq;
  std::vector<double> m_sx;
  std::vector<double> m_sp;
  std::vector<double> m_sq;
};

struct genotypes
{
  double m_x;
  double m_p;
  double m_q;
};

genotypes calc_average_genotype(const std::vector<indiv>& pop);

void calculate_rho(
    const std::vector<indiv>& pop,
    const genotypes& averageGenotypes,
    result_variables& result
    );

void calculate_s(
    const std::vector<indiv>& pop,
    const genotypes& averageGenotypes,
    result_variables& result
    );

void output_data(
    std::ofstream& out,
    const bigint t,
    const genotypes& averageGenotypes,
    const result_variables& result,
    const kewe_parameters& parameters
    );

void output(const bigint t,
            std::vector<std::vector<double>> &histX,
            std::vector<std::vector<double>> &histP,
            std::vector<std::vector<double>> &histQ,
            const kewe_parameters& parameters,
            const std::vector<indiv>& pop,
            result_variables& outputs
            );

void output_histogram(std::ofstream& out,
                 const std::vector<double>& hist,
                 std::vector<std::vector<double>>& hist_all_gens,
                 const double max,
                 const int histw
                 );

void output_histograms(
    std::ofstream& out,
    const kewe_parameters& parameters,
    const std::vector<indiv>& pop,
    std::vector<std::vector<double>> &histX,
    std::vector<std::vector<double>> &histP,
    std::vector<std::vector<double>> &histQ
    );

void count_num_border(
    const double l,
    const double o,
    const double r,
    int& numOfBorders);

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
