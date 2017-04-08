#ifndef JAAN_SIMULATION_H
#define JAAN_SIMULATION_H

#include <vector>
#include <random>
#include "jaan_parameters.h"
#include "jaan_habitat.h"
#include "jaan_individual.h"

class Simulation
{
public:
    Simulation();
    void run(
            std::mt19937& generator,
            const Parameters& p,
            const std::vector<Habitat>& habitat_list,
            const char stats_file[],
            const char hist_file[]);
    void output_data(
            std::ofstream& stats,
            std::ofstream& histograms,
            Parameters p,
            const std::vector<Individual>& population,
            const std::vector<int>& location,
            int g);
    void statistics(
            std::ofstream& stats,
            const std::vector<Individual>& population,
            const std::vector<int>& location);
    void histogram(
            std::ofstream& histograms,
            const Parameters& p,
            const std::vector<Individual>& population);
    std::vector<Individual> create_next_gen(
            std::mt19937& generator,
            const Parameters& p,
            const std::vector<Habitat>& habitat_list,
            const std::vector<Individual>& population,
            std::vector<int>& location);
    int pick_mother(
            std::mt19937& generator,
            const std::vector<double>& female_viab_dist);
    int pick_father(
            std::mt19937& generator,
            const Habitat& habitat,
            const std::vector<int> mates_in_hab,
            const std::vector<double>& quals,
            const std::vector<double>& male_viab_dist,
            const std::vector<Individual>& population,
            const double& m_pref);
    void crt_female_viability(
            const std::vector<Habitat>& habitat_list,
            const double& n_qual_genes,
            const std::vector<double>& preferences,
            const std::vector<double>& quals,
            const std::vector<int>& location,
            std::vector<double>& viab_dist);
    void crt_male_viability(
            const std::vector<Habitat>& habitat_list,
            const double& n_qual_genes,
            const std::vector<double>& traits,
            const std::vector<double>& quals,
            const std::vector<int>& location,
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
    void setup_initial_population(
            std::mt19937& generator,
            const Parameters& p,
            std::vector<Individual>& population,
            std::vector<int>& location);
    void setup_output_titles(
            std::ofstream& stats,
            std::ofstream& histograms,
            const Parameters& p,
            const std::vector<Habitat>& habitat_list,
            const std::vector<Individual>& population,
            const std::vector<int>& location,
            const std::vector<int>& location0);
};

void migration(
        std::mt19937& generator,
        const double& migration_rate,
        std::vector<int>& location);
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
double variance_calc(
        const std::vector<double>& v,
        const int pop);
double covariance_calc(
        const std::vector<double>& v1,
        const std::vector<double>& v2,
        const int pop);

#endif // JAAN_SIMULATION_H
