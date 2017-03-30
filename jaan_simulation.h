#ifndef JAAN_SIMULATION_H
#define JAAN_SIMULATION_H

#include <vector>
#include <random>
#include "jaan_parameters.h"
#include "jaan_individual.h"

class Simulation
{
public:
    Simulation();
    void run(
            std::mt19937& generator,
            const Parameters& p,
            const char stats_file[],
            const char hist_file[]);
    void statistics(
            std::ofstream& stats,
            const std::vector<Individual>& population);
    void histogram(
            std::ofstream& histograms,
            const Parameters& p,
            const std::vector<Individual>& population);
    std::vector<Individual> create_next_gen(
            std::mt19937& generator,
            const Parameters& p,
            const std::vector<Individual>& population);
    int pick_mother(
            std::mt19937& generator,
            const std::vector<double>& female_viab_dist);
    int pick_father(
            std::mt19937& generator,
            const Parameters& p,
            const std::vector<double>& quals,
            const std::vector<double>& male_viab_dist,
            const std::vector<Individual>& population,
            const double& m_pref);
    void crt_viability(
            const double& n_qual_genes,
            const std::vector<double>& ind_characters,
            const std::vector<double>& quals,
            const double& optimal_characters,
            const double& value_of_characters,
            const double& selection_on_quality,
            std::vector<double>& viab_dist);
    void mutate_populace(
            std::mt19937& generator,
            const Parameters& p,
            std::vector<Individual>& population);
    void mutate_pref_populace(
            std::mt19937& generator,
            const double& pref_mu,
            const double& scale_pref,
            std::vector<Individual>& population);
    void mutate_trt_populace(
            std::mt19937& generator,
            const double& trt_mu,
            const double& scale_trt,
            std::vector<Individual>& population);
    void mutate_qual_inc_populace(
            std::mt19937& generator,
            const double& max_quality,
            const double& qual_inc_mu,
            std::vector<Individual>& population);
    void mutate_qual_dec_populace(
            std::mt19937& generator,
            const double& qual_dec_mu,
            std::vector<Individual>& population);
};

void setup_histogram_titles(
        std::ofstream& histograms,
        const Parameters p);
void create_histogram(
        const int& n_genes,
        const double& ind_character,
        const double& scale,
        std::vector<double>& hist);
void output_histogram(
        std::ofstream& histograms,
        const double& n_genes,
        const std::vector<double>& hist);
std::vector<double> collect_prefs(const std::vector<Individual>& population);
std::vector<double> collect_trts(const std::vector<Individual>& population);
std::vector<double> collect_quals(const std::vector<Individual>& population);
double compute_square(const double& x);
double sum(const std::vector<double>& v);
std::vector<double> square_vector(const std::vector<double>& v);
double variance_calc(const std::vector<double>& v);
double covariance_calc(
        const std::vector<double>& v1,
        const std::vector<double>& v2);

#endif // JAAN_SIMULATION_H
