#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <stdexcept>
#include <algorithm>
#include <numeric>
#include <functional>
#include "jaan_simulation.h"
#include "jaan_parameters.h"

Simulation::Simulation()
{
}

/* Function for main running of the simulation, from initialising the first
 * population, collecting the stats and generating the next generation.
 */
void Simulation::run(
        std::mt19937& generator,
        const Parameters& p)
{
    const int pop_size = static_cast<int>(p.get_pop_size());
    /// Create a vector to store the population.
    std::vector<Individual> population(pop_size, Individual(p));
    for (int i = 0; i < pop_size; ++i) /// Randomise the population
    {
        population[i].init_population(generator, p);
    }
    /// give the initial population a chance of mutating before reproduction.
    double pref_mu = static_cast<double>(p.get_pref_mu());
    double trt_mu = static_cast<double>(p.get_trt_mu());
    double qual_inc = static_cast<double>(p.get_quality_inc_mu());
    double qual_dec = static_cast<double>(p.get_quality_dec_mu());
    const double scale_pref = static_cast<double>(p.get_scale_preference());
    const double scale_trt = static_cast<double>(p.get_scale_trait());
    mutate_pref_populace(generator, pref_mu, scale_pref, population);
    mutate_trt_populace(generator, trt_mu, scale_trt, population);
    mutate_qual_inc_populace(generator, qual_inc, population);
    mutate_qual_dec_populace(generator, qual_dec, population);
    std::ofstream stats("jaan_stats.csv");
    p.print_parameters(stats);
//    std::ofstream histograms("jaan_histograms.csv");
//    p.print_parameters(histograms);
    /// Put the column headers on the stats file.
    stats << "generation,mean_pref,mean_trt,mean_qual,pref_variance,"
          << "trt_variance,qual_variance,covariance,correlation\n0,";
    statistics(stats, population);
//    histograms << "generation,0" << std::endl;
//            histogram(histograms, p, population);
    for (int g = 0; g < p.get_max_generations(); ++g) /// Begin the loop.
    {
        std::cout << "generation " << g << std::endl;
        if (((g + 1) % 100) == 0) /// Only collect the stats every few generations.
        {
            stats << g << ',';
            statistics(stats, population);
//            histograms << "generation," << g << std::endl;
//            histogram(histograms, p, population);
        }
        population = create_next_gen(generator, p, population);
        mutate_pref_populace(generator, pref_mu, scale_pref, population);
        mutate_trt_populace(generator, trt_mu, scale_trt, population);
        mutate_qual_inc_populace(generator, qual_inc, population);
        mutate_qual_dec_populace(generator, qual_dec, population);
    }
    stats.close();
//    histograms.close();
}

/// Calculate the statistics and print them to the screen and to a stats file.
void Simulation::statistics(
        std::ofstream& stats,
        std::vector<Individual>& population)
{
    /// Create vectors for each of the traits to collect stats on them.
    const std::vector<double> prefs = collect_prefs(population);
    const std::vector<double> trts = collect_trts(population);
    const std::vector<double> quals = collect_quals(population);
    /// Calculate means.
    const double mean_pref = mean(prefs);
    const double mean_trt = mean(trts);
    const double mean_qual = mean(quals);
    /// Calculate the variances.
    const double pref_variance = variance_calc(prefs);
    const double trt_variance = variance_calc(trts);
    const double qual_variance = variance_calc(quals);
    /// Calculate the covariance and correlation between pref and trt.
    const double covariance = covariance_calc(prefs, trts);
    const double correlation = covariance / (pow(pref_variance, 0.5) * pow(trt_variance, 0.5));
    /// Print the stats to the screen.
    std::cout << "mean_pref " << mean_pref
              << " mean_trt " << mean_trt
              << " mean_qual " << mean_qual
              << " pref_variance " << pref_variance
              << " trt_variance " << trt_variance
              << " qual_variance " << qual_variance
              << " covariance " << covariance
              << " correlation " << correlation << std::endl;
    /// Print the stats to the file.
    stats << mean_pref << ','
          << mean_trt << ','
          << mean_qual << ','
          << pref_variance << ','
          << trt_variance << ','
          << qual_variance << ','
          << covariance << ','
          << correlation << std::endl;
}

/* Create two histograms of preferences and one of male traits in the population.
 * Possible values for the histogram run from all -1 to all +1 so the size is the difference
 * between the two, i.e. the all -1 state, all +1 state and the all 0 state.
 *
 * For some reason, the last time I tested this, the
 * create_histogram was not working for trt_genes.
 *
 */
void Simulation::histogram(
        std::ofstream& histograms,
        const Parameters& p,
        std::vector<Individual>& population)
{
    /// Declare some local variables so as not to have to call p so often.
    const int pop_size = static_cast<int>(p.get_pop_size());
    const double n_pref_genes = static_cast<double>(p.get_n_pref_genes());
    const double n_trt_genes = static_cast<double>(p.get_n_trt_genes());
    const double scale_preference = static_cast<double>(p.get_scale_preference());
    const double scale_trait = static_cast<double>(p.get_scale_trait());
    /// Declare the vectors to hold the histograms.
    std::vector<double> pref_hist(n_pref_genes + 1);
    std::vector<double> trt_hist(n_trt_genes + 1);
    for (int i = 0; i < pop_size; ++i) /// Create histograms for pref and trt using the function.
    {
        create_histogram(n_pref_genes, population[i].get_preference(),
                         scale_preference, pref_hist);
        create_histogram(n_trt_genes, population[i].get_trait(), scale_trait, trt_hist);
    }
    /// Declare some strings to pass to the function (Alternative would be two functions).
    const char pref_title1[] = "Preference_Value";
    const char pref_title2[] = "Preference_Histogram";
    const char trt_title1[] = "Trait_Value";
    const char trt_title2[] = "Trait_Histogram";
    /// Output the histograms to the screen and the histogram file using this function.
    output_histogram(histograms, n_pref_genes, pref_title1, pref_title2, pref_hist);
    output_histogram(histograms, n_trt_genes, trt_title1, trt_title2, trt_hist);
}

/// Create individuals by picking mothers and fathers.
///
/// Commented down to here.
std::vector<Individual> Simulation::create_next_gen(
        std::mt19937& generator,
        const Parameters& p,
        std::vector<Individual>& population)
{
    const int pop_size = static_cast<int>(p.get_pop_size());
    const double qual_viab = static_cast<double>(p.get_quality_viab());
    std::vector<Individual> offspring;
    offspring.reserve(pop_size);
    std::vector<double> female_viab_dist(pop_size);
    std::vector<double> prefs = collect_prefs(population);
    std::vector<double> quals = collect_quals(population);
    crt_viability(prefs, quals, p.get_optimal_preference(), p.get_selection_on_pref(),
                  qual_viab, female_viab_dist);
    std::vector<double> trts = collect_trts(population);
    std::vector<double> male_viab_dist(pop_size);
    crt_viability(trts, quals, p.get_optimal_trait(), p.get_selection_on_trt(),
                  qual_viab, male_viab_dist);
    for (int i = 0; i < pop_size; ++i)
    {
        const int mother = pick_mother(generator, female_viab_dist);
        const double m_pref = static_cast<double>(population[mother].get_preference());
        const int father = pick_father(generator, p, quals, male_viab_dist, population, m_pref);
        Individual child(generator, p, population[mother], population[father]);
        offspring.push_back(child);
    }
    return offspring;
}

/* This function calculates the viability of every individual as
 * a mother and then picks from them based on those viabilities.
 */
int Simulation::pick_mother(
        std::mt19937& generator,
        std::vector<double>& female_viab_dist)
{
    std::discrete_distribution<int> mother_distribution(female_viab_dist.begin(),
                                                        female_viab_dist.end());
    return mother_distribution(generator);
}

/* This function calculates the viability of every individual as a
 * father and their attractiveness to the individual passed to it.
 */
int Simulation::pick_father(
        std::mt19937& generator,
        const Parameters& p,
        std::vector<double>& quals,
        std::vector<double>& male_viab_dist,
        std::vector<Individual>& population,
        const double& m_pref)
{
    const int pop_size = static_cast<int>(p.get_pop_size());
    const double quality_attr = static_cast<double>(p.get_quality_attr());
    std::vector<double> attractivity(pop_size);
    for (int i = 0; i < pop_size; ++i)
    {
        attractivity[i] = male_viab_dist[i] *
                exp(m_pref * population[i].get_trait() * quality_attr * quals[i]);
    }
    std::discrete_distribution<int> father_distribution(attractivity.begin(), attractivity.end());
    return father_distribution(generator);
}

void Simulation::crt_viability(
        std::vector<double>& ind_characters,
        std::vector<double>& quals,
        const double& optimal_characters,
        const double& value_of_characters,
        const double& quality_viab,
        std::vector<double>& viab_dist)
{
    const int pop_size = ind_characters.size();
    for (int i = 0; i < pop_size; ++i)
    {
        double temp = (ind_characters[i] - optimal_characters) / value_of_characters;
        viab_dist[i] = pow((1 - quality_viab), (1 - quals[i])) * exp(-0.5 * temp * temp);
    }
}

void Simulation::mutate_pref_populace(
        std::mt19937& generator,
        const double& pref_mu,
        const double& scale_pref,
        std::vector<Individual>& population)
{
    std::poisson_distribution<int> mutation_count_dist(pref_mu);
    int n = mutation_count_dist(generator);
    std::uniform_int_distribution<int> pick_ind_dist(0, population.size() - 1);
    for (int i = 0; i < n; ++i)
    {
        population[pick_ind_dist(generator)].mutate_pref(generator, scale_pref);
    }
}

void Simulation::mutate_trt_populace(
        std::mt19937& generator,
        const double& trt_mu,
        const double& scale_trt,
        std::vector<Individual>& population)
{
    std::poisson_distribution<int> mutation_count_dist(trt_mu);
    int n = mutation_count_dist(generator);
    std::uniform_int_distribution<int> pick_ind_dist(0, population.size() - 1);
    for (int i = 0; i < n; ++i)
    {
        population[pick_ind_dist(generator)].mutate_trt(generator, scale_trt);
    }
}

void Simulation::mutate_qual_inc_populace(
        std::mt19937& generator,
        const double& qual_inc_mu,
        std::vector<Individual>& population)
{
    const int pop_size = static_cast<int>(population.size());
    std::poisson_distribution<int> mutation_count_dist(qual_inc_mu);
    int n = mutation_count_dist(generator);
    std::vector<double> quals = collect_quals(population);
    for (int i = 0; i < pop_size; ++i)
    {
        quals[i] = 1 - quals[i];
    }
    std::discrete_distribution<int> pick_ind_dist(quals.begin(), quals.end());
    for (int i = 0; i < n; ++i)
    {
        population[pick_ind_dist(generator)].mutate_qual_inc(generator);
    }
}

void Simulation::mutate_qual_dec_populace(
        std::mt19937& generator,
        const double& qual_dec_mu,
        std::vector<Individual>& population)
{
    std::poisson_distribution<int> mutation_count_dist(qual_dec_mu);
    int n = mutation_count_dist(generator);
    std::vector<double> quals = collect_quals(population);
    std::discrete_distribution<int> pick_ind_dist(quals.begin(), quals.end());
    for (int i = 0; i < n; ++i)
    {
        population[pick_ind_dist(generator)].mutate_qual_dec(generator);
    }
}

void create_histogram(
        const int& n_genes,
        const double& ind_character,
        const double& scale,
        std::vector<double>& hist)
{
    for (double h = 0 - n_genes; h < n_genes + 1; h += 2)
    {
        if (ind_character / scale * n_genes == h)
            ++hist[(h + n_genes) / 2];
    }
}

void output_histogram(
        std::ofstream& histograms,
        const double& n_genes,
        const char title1[],
        const char title2[],
        const std::vector<double>& hist)
{
    std::cout << title1 << ' ';
    histograms << title1 << ',';
    for (double h = 0 - n_genes; h < n_genes + 1; h += 2)
    {
        std::cout << h << ' ';
        histograms << h << ',';
    }
    std::cout << '\n' << title2 << ' ';
    histograms << '\n' << title2 << ',';
    const int hist_size = static_cast<int>(hist.size());
    for (int i = 0; i < hist_size; ++i)
    {
        std::cout << hist[i] << ' ';
        histograms << hist[i] << ',';
    }
    std::cout << std::endl;
    histograms << std::endl;
}

std::vector<double> collect_prefs(const std::vector<Individual>& population)
{
    int pop_size = static_cast<int>(population.size());
    std::vector<double> v(pop_size);
    for (int i = 0; i < pop_size; ++i)
    {
        v[i] = population[i].get_preference();
    }
    return v;
}

std::vector<double> collect_trts(const std::vector<Individual>& population)
{
    int pop_size = static_cast<int>(population.size());
    std::vector<double> v(pop_size);
    for (int i = 0; i < pop_size; ++i)
    {
        v[i] = population[i].get_trait();
    }
    return v;
}

std::vector<double> collect_quals(const std::vector<Individual>& population)
{
    int pop_size = static_cast<int>(population.size());
    std::vector<double> v(pop_size);
    for (int i = 0; i < pop_size; ++i)
    {
        v[i] = population[i].get_quality();
    }
    return v;
}

double sum(const std::vector<double>& v)
{
    return std::accumulate(
                std::begin(v),
                std::end(v),
                0.0
            );
}

std::vector<double> square_vector(const std::vector<double>& v)
{
    int pop_size = static_cast<int>(v.size());
    std::vector<double> n(pop_size);
    std::transform(v.begin(), v.end(), v.begin(), n.begin(), std::multiplies<double>());
    return n;
}

double variance_calc(const std::vector<double>& v)
{
    const double vector_size = static_cast<double>(v.size());
    return (sum(square_vector(v)) - (sum(v)  * sum(v)) / vector_size) / vector_size;
}

double covariance_calc(
        const std::vector<double>& v1,
        const std::vector<double>& v2)
{
    if (v1.size() != v2.size())
    {
        throw std::invalid_argument("Covariance vectors must be equal in size.");
    }
    const double vector_size = static_cast<double>(v1.size());
    std::vector<double> multiplier(vector_size);
    std::transform(std::begin(v1), std::end(v1), std::begin(v2), std::begin(multiplier),
                   std::multiplies<double>());
    return (sum(multiplier) - ((sum(v1) * sum(v2)) / vector_size)) / vector_size;
}
