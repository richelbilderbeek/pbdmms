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
        const Parameters& p,
        const char stats_file[],
        const char hist_file[])
{
    /// Declare some local variables to use rather than calling the Parameters function.
    const int pop_size = static_cast<int>(p.get_pop_size());
    /// Create a vector to store the population.
    std::vector<Individual> population(pop_size, Individual(p));
    /// Create a vector to store the location of every individual.
    std::vector<int> location(pop_size, 0);
    /// Create the probability that each individual will be in habitat 0 or 1.
    std::uniform_real_distribution<double> habitat_dist(0.0,1.0);
    for (int i = 0; i < pop_size; ++i)
    {
        population[i].init_genetics(generator); /// Randomise the population.
        if (habitat_dist(generator) < 0.5) /// Randomly put half the population in habitat 1.
        {
            location[i] = 1;
        }
    }
    /// give the initial population a chance of mutating before reproduction.
    mutate_populace(generator, p, population);
    std::ofstream stats(stats_file); /// Create an output file to store the statistics.
    p.print_parameters(stats); /// Print the parameters to the stats file.
    /// Create an output file to store the histograms
    std::ofstream histograms(hist_file);
//    p.print_parameters(histograms); /// Print the parameters to the histograms file.
    /// Put the column headers on the stats file.
    stats << "generation,mean_pref,mean_trt,mean_qual,pref_variance,"
          << "trt_variance,qual_variance,covariance,correlation\n0," << std::endl;
    statistics(stats, population);
    /// Print the relevant headers to the histogram file.
//    setup_histogram_titles(histograms, p);
//    histograms << "0,";
//    histogram(histograms, p, population);
    std::cout << "n of habitat 1: " << std::accumulate(
                                           std::begin(location),
                                           std::end(location),
                                           0.0
                                       ) << std::endl;
    for (int g = 0; g < p.get_max_generations(); ++g) /// Begin the generational loop.
    {
        std::cout << "generation " << g << std::endl;
//        if (((g + 1) % 100) == 0) /// Only collect the stats every few generations.
        {
            stats << g << ',';
            statistics(stats, population);
//            histograms << g << '\t';
//            histogram(histograms, p, population);
        }
        /// Create the new generation and assign it as the current generation.
        population = create_next_gen(generator, p, population, location);
        std::cout << "n of habitat 1: " << std::accumulate(
                                               std::begin(location),
                                               std::end(location),
                                               0.0
                                           ) << std::endl;
    }
    stats.close(); /// Close the stats output file
    histograms.close(); /// Close the histogram output file
}

/// Calculate the statistics and print them to the screen and to a stats file.
void Simulation::statistics(
        std::ofstream& stats,
        const std::vector<Individual>& population)
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
void Simulation::histogram(
        std::ofstream& histograms,
        const Parameters& p,
        const std::vector<Individual>& population)
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
    /// Output the histograms to the screen and the histogram file using this function.
    output_histogram(histograms, n_pref_genes, pref_hist);
    histograms << ',';
    output_histogram(histograms, n_trt_genes, trt_hist);
    histograms << std::endl;
}



/// Choose which individuals will switch between each habitat.



/// Create individuals by picking mothers and fathers.
std::vector<Individual> Simulation::create_next_gen(
        std::mt19937& generator,
        const Parameters& p,
        const std::vector<Individual>& population,
        std::vector<int>& location)
{
    const int pop_size = static_cast<int>(p.get_pop_size());
    const double selection_on_quality = static_cast<double>(p.get_selection_on_quality());
    const double n_qual_genes = static_cast<double>(p.get_n_qual_genes());
    std::vector<Individual> offspring; /// Create a vector to store the new individuals until end.
    offspring.reserve(pop_size);
    std::vector<int> offspring_location(pop_size); /// Place to store the new locations.
    /// Creating the viability distributions for this generation as females.
    std::vector<double> female_viab_dist(pop_size);
    std::vector<double> prefs = collect_prefs(population);
    std::vector<double> quals = collect_quals(population);
    crt_viability(n_qual_genes, prefs, quals, p.get_optimal_preference(),
                  p.get_selection_on_pref(), selection_on_quality, female_viab_dist);
    /// Creating the viability distributions for this generation as males.
    std::vector<double> trts = collect_trts(population);
    std::vector<double> male_viab_dist(pop_size);
    crt_viability(n_qual_genes, trts, quals, p.get_optimal_trait(),
                  p.get_selection_on_trt(), selection_on_quality, male_viab_dist);
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
        /// Assign the location of the mother to the location of the individual.
        offspring_location[i] = location[mother];
    }
    /// Give each individual a chance of mutating.
    mutate_populace(generator, p, offspring);
    /// Assign the offspring's location to the main location vector.
    location = offspring_location;
    return offspring;
}

/// Picks the position of a mother in the population based on her viability.
int Simulation::pick_mother(
        std::mt19937& generator,
        const std::vector<double>& female_viab_dist)
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
    const double expr_efficiency = static_cast<double>(p.get_expr_efficiency());
    /// Create a vector of the right size to hold the attractiveness values.
    std::vector<double> attractivity(pop_size);
    for (int i = 0; i < pop_size; ++i)
    {
        /// Attractiveness is Vm * exp(mother's pref * my trait * quality investment * quality
        attractivity[i] = male_viab_dist[i] *
                exp(m_pref * population[i].get_trait() * expr_efficiency);// * quals[i]);
    }
    std::discrete_distribution<int> father_distribution(attractivity.begin(), attractivity.end());
    return father_distribution(generator);


    /// Remove when quality is included again.
    return quals[1];
}

/// Calculate the viability of an individual given either a trait or a choosiness.
void Simulation::crt_viability(
        const double& n_qual_genes,
        const std::vector<double>& ind_characters,
        const std::vector<double>& quals,
        const double& optimal_characters,
        const double& selection_on_char,
        const double& selection_on_quality,
        std::vector<double>& viab_dist)
{
    const int pop_size = ind_characters.size();
    for (int i = 0; i < pop_size; ++i)
    {
        /// Viability is the quality effect ^ (missing quality) *
        /// exp( -1/2 * ( character - neutral / selection ) ^ 2 )
        const double temp = (ind_characters[i] - optimal_characters) / selection_on_char;
        viab_dist[i] = pow((1 - selection_on_quality), (1 - (quals[i] / n_qual_genes))) *
                exp(-0.5 * temp * temp);
    }
}


/// Function for calling all of the mutation functions.
void Simulation::mutate_populace(
        std::mt19937& generator,
        const Parameters& p,
        std::vector<Individual>& population)
{
    const double pref_mu = static_cast<double>(p.get_pref_and_trt_mu() * p.get_n_pref_genes());
    const double scale_pref = static_cast<double>(p.get_scale_preference());
    const double trt_mu = static_cast<double>(p.get_pref_and_trt_mu() * p.get_n_trt_genes());
    const double scale_trt = static_cast<double>(p.get_scale_trait());
    const double qual_inc = static_cast<double>(p.get_quality_inc_mu());
    const double max_quality = static_cast<double>(p.get_n_qual_genes());
    const double qual_dec = static_cast<double>(p.get_quality_dec_mu());
    mutate_pref_populace(generator, pref_mu, scale_pref, population);
    mutate_trt_populace(generator, trt_mu, scale_trt, population);
    mutate_qual_inc_populace(generator, max_quality, qual_inc, population);
    mutate_qual_dec_populace(generator, qual_dec, population);
}

/// Calculate the number of individuals that should receive a mutation
/// of preference and randomly pick individuals to receive the mutation.
void Simulation::mutate_pref_populace(
        std::mt19937& generator,
        const double& pref_mu,
        const double& scale_pref,
        std::vector<Individual>& population)
{
    int pop_size = static_cast<int>(population.size());
    std::uniform_real_distribution<double> mutation_dist(0.0,1.0);
    for (int i = 0; i < pop_size; ++i)
    {
        if (mutation_dist(generator) < pref_mu)
        {
            population[i].mutate_pref(generator, scale_pref);
        }
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
    int pop_size = static_cast<int>(population.size());
    std::uniform_real_distribution<double> mutation_dist(0.0,1.0);
    for (int i = 0; i < pop_size; ++i)
    {
        if (mutation_dist(generator) < trt_mu)
        {
            population[i].mutate_trt(generator, scale_trt);
        }
    }
}

/// Calculate the number of individuals that should receive a beneficial mutation
/// of quality and randomly pick individuals to receive the mutation.
/* Make sure the tests of the mutations is robust.
 */
void Simulation::mutate_qual_inc_populace(
        std::mt19937& generator,
        const double& max_quality,
        const double& qual_inc_mu,
        std::vector<Individual>& population)
{
    int pop_size = static_cast<int>(population.size());
    std::uniform_real_distribution<double> mutation_dist(0.0,1.0);
    for (int i = 0; i < pop_size; ++i)
    {
        /// Chance is proportional to number of bad quality genes individual i has.
        if (mutation_dist(generator) < (qual_inc_mu * (max_quality - population[i].get_quality())))
        {
            population[i].mutate_qual_inc(generator);
        }
    }
}

/// Calculate the number of individuals that should receive a detrimental mutation
/// of quality and randomly pick individuals to receive the mutation.
void Simulation::mutate_qual_dec_populace(
        std::mt19937& generator,
        const double& qual_dec_mu,
        std::vector<Individual>& population)
{
    int pop_size = static_cast<int>(population.size());
    std::uniform_real_distribution<double> mutation_dist(0.0,1.0);
    for (int i = 0; i < pop_size; ++i)
    {
        /// Chance is proportional to number of good quality genes individual i has.
        if (mutation_dist(generator) < (qual_dec_mu * population[i].get_quality()))
        {
            population[i].mutate_qual_dec(generator);
        }
    }
}

/// print the titles of the histogram columns once to the file.
void setup_histogram_titles(
        std::ofstream& histograms,
        const Parameters p)
{
    /// paste the title of the first two sections
    histograms << "generation,pref_hist,";
    const int n_pref_genes = static_cast<int>(p.get_n_pref_genes());
    /// paste the title of the second histogram above the start of the second histogram.
    histograms << ',' * n_pref_genes << "trt_hist";
    /// Paste the range of values that the preference histogram could have.
    for (int i = 0 - n_pref_genes ; i < n_pref_genes; ++i)
    {
        histograms << i << ',';
    }
    histograms << ','; /// Put a gap between the two histograms.
    const int n_trt_genes = static_cast<int>(p.get_n_trt_genes());
    /// Paste the range of values that the trait histogram could have.
    for (int i = 0 - n_trt_genes ; i < n_trt_genes; ++i)
    {
        histograms << i << ',';
    }
    histograms << std::endl;
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
        const std::vector<double>& hist)
{
    /// Paste the values that each position is.
    for (double h = 0 - n_genes; h < n_genes + 1; h += 2)
    {
        histograms << h << ',';
    }
    const int hist_size = static_cast<int>(hist.size());
    /// Paste the frequencies of each value.
    for (int i = 0; i < hist_size; ++i)
    {
        histograms << hist[i] << ',';
    }
}

/// Create a vector of preferences as a look-up for some functions.
std::vector<double> collect_prefs(const std::vector<Individual>& population)
{
    const int pop_size = static_cast<int>(population.size());
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
    const int pop_size = static_cast<int>(population.size());
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
    const int pop_size = static_cast<int>(population.size());
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
