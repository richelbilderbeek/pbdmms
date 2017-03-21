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

/// Running of the simulation, from initialising the first population,
/// collecting the stats and generating the next generation.
void Simulation::run(
        std::mt19937& generator,
        const Parameters& p)
{
    /// Declare some local variables to use rather than calling the Parameters function.
    const int pop_size = static_cast<int>(p.get_pop_size());
    double pref_mu = static_cast<double>(p.get_pref_mu());
    double trt_mu = static_cast<double>(p.get_trt_mu());
    double qual_inc = static_cast<double>(p.get_quality_inc_mu());
    double qual_dec = static_cast<double>(p.get_quality_dec_mu());
    const double scale_pref = static_cast<double>(p.get_scale_preference());
    const double scale_trt = static_cast<double>(p.get_scale_trait());
    /// Create a vector to store the population.
    std::vector<Individual> population(pop_size, Individual(p));
    for (int i = 0; i < pop_size; ++i) /// Randomise the population
    {
        population[i].init_population(generator, p);
    }
    /// give the initial population a chance of mutating before reproduction.
    mutate_pref_populace(generator, pref_mu, scale_pref, population);
    mutate_trt_populace(generator, trt_mu, scale_trt, population);
    mutate_qual_inc_populace(generator, qual_inc, population);
    mutate_qual_dec_populace(generator, qual_dec, population);
    std::ofstream stats("jaan_stats.csv"); /// Create an output file to store the statistics.
    p.print_parameters(stats); /// Print the parameters to the stats file.
    /// Create an output file to store the histograms
//    std::ofstream histograms("jaan_histograms.csv");
//    p.print_parameters(histograms);
    /// Put the column headers on the stats file.
    stats << "generation,mean_pref,mean_trt,mean_qual,pref_variance,"
          << "trt_variance,qual_variance,covariance,correlation\n0,";
    statistics(stats, population);
//    histograms << "generation,0" << std::endl;
//            histogram(histograms, p, population);
    for (int g = 0; g < p.get_max_generations(); ++g) /// Begin the generational loop.
    {
        std::cout << "generation " << g << std::endl;
        if (((g + 1) % 100) == 0) /// Only collect the stats every few generations.
        {
            stats << g << ',';
            statistics(stats, population);
//            histograms << "generation," << g << std::endl;
//            histogram(histograms, p, population);
        }
        /// Create the new generation and assign it as the current generation.
        population = create_next_gen(generator, p, population);
        /// Mutate the generation's genes a little.
        mutate_pref_populace(generator, pref_mu, scale_pref, population);
        mutate_trt_populace(generator, trt_mu, scale_trt, population);
        mutate_qual_inc_populace(generator, qual_inc, population);
        mutate_qual_dec_populace(generator, qual_dec, population);
    }
    stats.close(); /// Close the stats output file
//    histograms.close(); /// Close the histogram output file
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

/// Create two histograms of preferences and one of male traits in the population.
/// Possible values for the histogram run from all -1 to all +1 so the size is the difference
/// between the two, i.e. the all -1 state, all +1 state and the all 0 state.
/*
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
std::vector<Individual> Simulation::create_next_gen(
        std::mt19937& generator,
        const Parameters& p,
        std::vector<Individual>& population)
{
    const int pop_size = static_cast<int>(p.get_pop_size());
    const double qual_viab = static_cast<double>(p.get_quality_viab());
    const double n_qual_genes = static_cast<double>(p.get_n_qual_genes());
    std::vector<Individual> offspring; /// Create a vector to store the new individuals until end.
    offspring.reserve(pop_size);
    /// Creating the viability distributions for this generation as females.
    std::vector<double> female_viab_dist(pop_size);
    std::vector<double> prefs = collect_prefs(population);
    std::vector<double> quals = collect_quals(population);
    crt_viability(n_qual_genes, prefs, quals, p.get_optimal_preference(),
                  p.get_selection_on_pref(), qual_viab, female_viab_dist);
    /// Creating the viability distributions for this generation as males.
    std::vector<double> trts = collect_trts(population);
    std::vector<double> male_viab_dist(pop_size);
    crt_viability(n_qual_genes, trts, quals, p.get_optimal_trait(),
                  p.get_selection_on_trt(), qual_viab, male_viab_dist);
    /// Choosing individuals to create the next generation.
    for (int i = 0; i < pop_size; ++i)
    {
        /// Choose a mother
        const int mother = pick_mother(generator, female_viab_dist);
        /// Pass the mother's preference to the father choosing function.
        const double m_pref = static_cast<double>(population[mother].get_preference());
        /// Use mother to choose the father.
        const int father = pick_father(generator, p, quals, male_viab_dist, population, m_pref);
        /// Create the individual.
        Individual child(generator, p, population[mother], population[father]);
        /// Assign the new individual to the offspring vector.
        offspring.push_back(child);
    }
    return offspring;
}

/// Picks the position of a mother in the population based on her viability.
int Simulation::pick_mother(
        std::mt19937& generator,
        std::vector<double>& female_viab_dist)
{
    std::discrete_distribution<int> mother_distribution(female_viab_dist.begin(),
                                                        female_viab_dist.end());
    return mother_distribution(generator);
}

/// Calculates the attractiveness of every individual to the individual passed to it.
int Simulation::pick_father(
        std::mt19937& generator,
        const Parameters& p,
        const std::vector<double>& quals,
        const std::vector<double>& male_viab_dist,
        const std::vector<Individual>& population,
        const double& m_pref)
{
    const int pop_size = static_cast<int>(p.get_pop_size());
    const double quality_attr = static_cast<double>(p.get_quality_attr());
    /// Create a vector of the right size to hold the attractiveness values.
    std::vector<double> attractivity(pop_size);
    for (int i = 0; i < pop_size; ++i)
    {
        /// Attractiveness is Vm * exp(mother's pref * my trait * quality investment * quality
        attractivity[i] = male_viab_dist[i] *
                exp(m_pref * population[i].get_trait() * quality_attr);// * quals[i]);
    }
    std::discrete_distribution<int> father_distribution(attractivity.begin(), attractivity.end());
    return father_distribution(generator);
    return quals[1];
}

/// Calculate the viability of an individual given either a trait or a choosiness.
void Simulation::crt_viability(
        const double& n_qual_genes,
        const std::vector<double>& ind_characters,
        const std::vector<double>& quals,
        const double& optimal_characters,
        const double& value_of_characters,
        const double& quality_viab,
        std::vector<double>& viab_dist)
{
    const int pop_size = ind_characters.size();
    for (int i = 0; i < pop_size; ++i)
    {
        /// Viability is the quality effect ^ quality *
        /// exp( -1/2 * ( character - neutral / selection ) ^ 2 )
        double temp = (ind_characters[i] - optimal_characters) / value_of_characters;
        viab_dist[i] = pow((1 - quality_viab), (quals[i] / n_qual_genes)) * exp(-0.5 * temp * temp);
    }
}

/// Calculate the number of individuals that should receive a mutation
/// of preference and randomly pick individuals to receive the mutation.
void Simulation::mutate_pref_populace(
        std::mt19937& generator,
        const double& pref_mu,
        const double& scale_pref,
        std::vector<Individual>& population)
{
    /// Number of mutations is poisson distributed around the mean.
    std::poisson_distribution<int> mutation_count_dist(pref_mu);
    int n = mutation_count_dist(generator);
    /// Choose each individual with equal probability.
    std::uniform_int_distribution<int> pick_ind_dist(0, population.size() - 1);
    for (int i = 0; i < n; ++i)
    {
        int j = pick_ind_dist(generator);
        population[j].mutate_pref(generator, scale_pref);
    }
}

/// Calculate the number of individuals that should receive a mutation
/// of trait and randomly pick individuals to receive the mutation.
void Simulation::mutate_trt_populace(
        std::mt19937& generator,
        const double& trt_mu,
        const double& scale_trt,
        std::vector<Individual>& population)
{
    /// Number of mutations is poisson distributed around the mean.
    std::poisson_distribution<int> mutation_count_dist(trt_mu);
    int n = mutation_count_dist(generator);
    /// Choose each individual with equal probability.
    std::uniform_int_distribution<int> pick_ind_dist(0, population.size() - 1);
    for (int i = 0; i < n; ++i)
    {
        int j = pick_ind_dist(generator);
        population[j].mutate_trt(generator, scale_trt);
    }
}

/// Calculate the number of individuals that should receive a beneficial mutation
/// of quality and randomly pick individuals to receive the mutation.
void Simulation::mutate_qual_inc_populace(
        std::mt19937& generator,
        const double& qual_inc_mu,
        std::vector<Individual>& population)
{
    const int pop_size = static_cast<int>(population.size());
    /// Number of mutations is poisson distributed around the mean.
    std::poisson_distribution<int> mutation_count_dist(qual_inc_mu);
    int n = mutation_count_dist(generator);
    /// Reverse the quals to create quality weightings based on number of
    /// genes that have a chance to mutate in the relevant direction.
    std::vector<double> quals = collect_quals(population);
    for (int i = 0; i < pop_size; ++i)
    {
        quals[i] = 1 - quals[i];
    }
    /// Choose individuals based on the number of genes they
    /// have that can mutate in the relevant direction.
    std::discrete_distribution<int> pick_ind_dist(quals.begin(), quals.end());
    for (int i = 0; i < n; ++i)
    {
        population[pick_ind_dist(generator)].mutate_qual_inc(generator);
    }
}

/// Calculate the number of individuals that should receive a detrimental mutation
/// of quality and randomly pick individuals to receive the mutation.
void Simulation::mutate_qual_dec_populace(
        std::mt19937& generator,
        const double& qual_dec_mu,
        std::vector<Individual>& population)
{
    /// Number of mutations is poisson distributed around the mean.
    std::poisson_distribution<int> mutation_count_dist(qual_dec_mu);
    int n = mutation_count_dist(generator);
    std::vector<double> quals = collect_quals(population);
    /// Choose individuals based on the number of genes they
    /// have that can mutate in the relevant direction.
    std::discrete_distribution<int> pick_ind_dist(quals.begin(), quals.end());
    for (int i = 0; i < n; ++i)
    {
        population[pick_ind_dist(generator)].mutate_qual_dec(generator);
    }
}

/// Adds one to a position in the vector for each individual with the correct value.
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

/// Puts histogram data into the histograms output file.
void output_histogram(
        std::ofstream& histograms,
        const double& n_genes,
        const char title1[],
        const char title2[],
        const std::vector<double>& hist)
{
    std::cout << title1 << ' '; /// Paste which histogram this is and the title of this row.
    histograms << title1 << ',';
    /// Paste the values that each position is.
    for (double h = 0 - n_genes; h < n_genes + 1; h += 2)
    {
        std::cout << h << ' ';
        histograms << h << ',';
    }
    std::cout << '\n' << title2 << ' '; /// Paste the title of this row.
    histograms << '\n' << title2 << ',';
    const int hist_size = static_cast<int>(hist.size());
    /// Paste the frequencies of each value.
    for (int i = 0; i < hist_size; ++i)
    {
        std::cout << hist[i] << ' ';
        histograms << hist[i] << ',';
    }
    std::cout << std::endl;
    histograms << std::endl;
}

/// Create a vector of preferences as a look-up for some functions.
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

/// Create a vector of traits as a look-up for some functions.
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

/// Create a vector of qualities as a look-up for some functions.
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

/// Sum helper function.
double sum(const std::vector<double>& v)
{
    return std::accumulate(
                std::begin(v),
                std::end(v),
                0.0
            );
}

/// Squares each value in a vector, helper function.
std::vector<double> square_vector(const std::vector<double>& v)
{
    int pop_size = static_cast<int>(v.size());
    std::vector<double> n(pop_size);
    std::transform(v.begin(), v.end(), v.begin(), n.begin(), std::multiplies<double>());
    return n;
}

/// Calculates the variance of a vector.
double variance_calc(const std::vector<double>& v)
{
    const double vector_size = static_cast<double>(v.size());
    return (sum(square_vector(v)) - (sum(v)  * sum(v)) / vector_size) / vector_size;
}

/// Calculates the covariance of two vectors.
double covariance_calc(
        const std::vector<double>& v1,
        const std::vector<double>& v2)
{
    /// Check the two vectors are the same size.
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
