#ifndef KEWE_RESULTS_H
#define KEWE_RESULTS_H

#include <iostream>
#include <vector>
#include "kewe_parameters.h"
#include "kewe_individual.h"
#include <boost/graph/adjacency_list.hpp>

namespace kewe {

///For visualization
using histograms_in_time = std::vector<std::vector<double>>;
using ltt = std::vector<std::pair<int, int>>;

struct results
{
  histograms_in_time m_ecological_trait;
  histograms_in_time m_female_preference;
  histograms_in_time m_male_trait;
  ltt m_ltt;
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


void add_vertexes(
    const std::vector<indiv>& pop,
    boost::adjacency_list<
    boost::vecS, boost::vecS, boost::undirectedS, std::string
    >& g
    );

void add_vertices(
    const std::vector<indiv>& pop,
    const std::vector<std::vector<double>>& attractiveness_pop,
    boost::adjacency_list<
    boost::vecS, boost::vecS, boost::undirectedS, std::string
    >& g,
    const parameters& parameters
    );

std::vector<std::vector<double>> calc_attractiveness_indivs(
                                   const std::vector<indiv>& pop,
                                   const parameters& p
                                   );
  
genotypes calc_average_genotype(const std::vector<indiv>& pop);

int calc_j_trait(const int histw, const double trait, const parameters& parameters);

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

int count_good_species(
    const std::vector<indiv>& pop,
    const parameters& parameters
    );

void output_data(
    std::ofstream& out,
    const bigint t,
    const genotypes& averageGenotypes,
    const result_variables& result,
    const parameters& parameters
    );

void output(const bigint t,
            std::vector<std::vector<double>> &histX,
            std::vector<std::vector<double>> &histP,
            std::vector<std::vector<double>> &histQ,
            const parameters& parameters,
            const std::vector<indiv>& pop,
            result_variables& result,
            std::vector<std::pair<bigint,int>>& ltt_plot
            );

void output_histogram(std::ofstream& out,
                 const std::vector<double>& hist,
                 std::vector<std::vector<double>>& hist_all_gens,
                 const int histw
                 );

void output_histograms(
    std::ofstream& out,
    const parameters& parameters,
    const std::vector<indiv>& pop,
    std::vector<std::vector<double>> &histX,
    std::vector<std::vector<double>> &histP,
    std::vector<std::vector<double>> &histQ
    );

void output_ltt(
    const std::vector<indiv>& pop,
    const bigint t,
    const parameters& p,
    std::vector<std::pair<bigint,int>>& ltt_plot
    );
/*
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

void throw_count_lineages(const int t,
                          const std::vector<std::vector<double>>& histX,
                          const std::vector<std::vector<double>>& histP,
                          const std::vector<std::vector<double>>& histQ
                          );
*/

} //~namespace kewe

#endif // KEWE_RESULTS_H
