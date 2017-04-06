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

Simulation::Simulation()
{
}

/// Running of the simulation, from initialising the first population,
/// collecting the stats and generating the next generation.
void Simulation::run(
        std::mt19937& generator,
        const Parameters& p,
        const std::vector<Habitat>& habitat_list,
        const char stats_file[],
        const char hist_file[])
{
    /// Declare some local variables to use rather than calling the Parameters function.
    const int pop_size = static_cast<int>(p.get_pop_size());
    const double migration_rate = static_cast<double>(p.get_migration_rate());
    /// Create a vector to store the population.
    std::vector<Individual> population(pop_size, Individual(p));
    /// Create a vector to store the location of every individual.
    std::vector<int> location(pop_size, 0);
    std::vector<int> location0(pop_size);
    for (int i = 0; i < pop_size; ++i)
    {
        location0[i] = !location[i];
    }
    std::ofstream stats(stats_file); /// Create an output file to store the statistics.
    std::ofstream histograms(hist_file); /// Create an output file to store the histograms.
    setup_initial_population(generator, p, population, location);
    setup_output_titles(stats, histograms, p, habitat_list, population, location, location0);
    for (int g = 0; g < p.get_max_generations(); ++g) /// Begin the generational loop.
    {
        std::cout << "generation " << g << ',';//std::endl;
        if (((g + 1) % 100) == 0) /// Only collect the stats every few generations.
        {
            output_data(stats, histograms, population, location);
        }
        /// Create the new generation and assign it as the current generation.
        population = create_next_gen(generator, p, habitat_list, population, location);
        migration(generator, migration_rate, location);
    }
    stats.close(); /// Close the stats output file
    histograms.close(); /// Close the histogram output file
}

void Simulation::output_data(
        std::ofstream& stats,
        std::ofstream& histograms,
        const std::vector<Individual>& population,
        const std::vector<int>& location)
{
    stats << g << ',';
    /// Create an inverted location vector to use for stats collecting.
    std::vector<int> location0(pop_size);
    for (int i = 0; i < pop_size; ++i)
    {
        location0[i] = !location[i];
    }
    /// Collect stats for the population in each location.
    statistics(stats, population, location0);
    stats << g << ',';
    statistics(stats, population, location);
    stats << std::endl;
    histograms << g << '\t';
    histogram(histograms, p, population);
}

/// Calculate the statistics and print them to the screen and to a stats file.
void Simulation::statistics(
        std::ofstream& stats,
        const std::vector<Individual>& population,
        const std::vector<int>& location)
{
    const int pop_size = static_cast<int>(population.size());
    /// Calculate the number of individuals in the focal habitat;
    const double pop = std::accumulate(std::begin(location), std::end(location), 0.0);
    /// Create vectors for each of the traits to collect stats on them.
    std::vector<double> prefs = collect_prefs(population);
    std::vector<double> trts = collect_trts(population);
    std::vector<double> quals = collect_quals(population);
    for (int i = 0; i < pop_size; ++i)
    {
        prefs[i] *= location[i];
        trts[i] *= location[i];
        quals[i] *= location[i];
    }
    /// Calculate means.
    const double mean_pref = std::accumulate(std::begin(prefs), std::end(prefs), 0.0) / pop;
    const double mean_trt = std::accumulate(std::begin(trts), std::end(trts), 0.0) / pop;
    const double mean_qual = std::accumulate(std::begin(quals), std::end(quals), 0.0) / pop;
    /// Calculate the variances.
    const double pref_variance = variance_calc(prefs, pop);
    const double trt_variance = variance_calc(trts, pop);
    const double qual_variance = variance_calc(quals, pop);
    /// Calculate the covariance and correlation between pref and trt.
    const double covariance = covariance_calc(prefs, trts, pop);
    const double correlation = covariance / (pow(pref_variance, 0.5) * pow(trt_variance, 0.5));
    /// Print the stats to the screen.
/*    std::cout << "habitat_pop " << pop
              << " mean_pref " << mean_pref
              << " mean_trt " << mean_trt
              << " mean_qual " << mean_qual
              << " pref_variance " << pref_variance
              << " trt_variance " << trt_variance
              << " qual_variance " << qual_variance
              << " covariance " << covariance
              << " correlation " << correlation<< std::endl;
*/    /// Print the stats to the file.
    stats << pop << ','
          << mean_pref << ','
          << mean_trt << ','
          << mean_qual << ','
          << pref_variance << ','
          << trt_variance << ','
          << qual_variance << ','
          << covariance << ','
          << correlation << ',';
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

/// Create individuals by picking mothers and fathers.
std::vector<Individual> Simulation::create_next_gen(
        std::mt19937& generator,
        const Parameters& p,
        const std::vector<Habitat>& habitat_list,
        const std::vector<Individual>& population,
        std::vector<int>& location)
{
    const int pop_size = static_cast<int>(p.get_pop_size());
    const double n_qual_genes = static_cast<double>(p.get_n_qual_genes());
    std::vector<Individual> offspring; /// Create a vector to store the new individuals until end.
    offspring.reserve(pop_size);
    std::vector<int> offspring_location(pop_size); /// Place to store the new locations.
    /// Creating the viability distributions for this generation as females.
    std::vector<double> female_viab_dist(pop_size);
    std::vector<double> prefs = collect_prefs(population);
    std::vector<double> quals = collect_quals(population);
    crt_female_viability(habitat_list, n_qual_genes, prefs, quals, location, female_viab_dist);
    /// Creating the viability distributions for this generation as males.
    std::vector<double> trts = collect_trts(population);
    std::vector<double> male_viab_dist(pop_size);
    crt_male_viability(habitat_list, n_qual_genes, trts, quals, location, male_viab_dist);
    /// Choosing individuals to create the next generation.
    for (int i = 0; i < pop_size; ++i)
    {
        /// Choose a mother
        const int mother = pick_mother(generator, female_viab_dist);
        /// Pass the mother's preference to the father choosing function.
        const double m_pref = static_cast<double>(population[mother].get_preference());
        /// Use mother to choose the father.
        const int father = pick_father(generator, habitat_list, location, quals, male_viab_dist, population, m_pref);
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
        const std::vector<Habitat>& habitat_list,
        const std::vector<int>& location,
        const std::vector<double>& quals,
        const std::vector<double>& male_viab_dist,
        const std::vector<Individual>& population,
        const double& m_pref)
{
    const int pop_size = static_cast<int>(population.size());
    /// Create a vector of the right size to hold the attractiveness values.
    std::vector<double> attractivity(pop_size);
    for (int i = 0; i < pop_size; ++i)
    {
        /// Attractiveness is Vm * exp(mother's pref * my trait * quality investment * quality
        attractivity[i] = male_viab_dist[i] *
                exp(m_pref * population[i].get_trait() *
                    habitat_list[location[i]].get_signal_clarity() *
                    habitat_list[location[i]].get_expr_efficiency());// * quals[i]);
    }
    std::discrete_distribution<int> father_distribution(attractivity.begin(), attractivity.end());
    return father_distribution(generator);


    /// Remove when quality is included again.
    return quals[1];
}

/// Calculate the viability of an individual given a choosiness.
void Simulation::crt_female_viability(
        const std::vector<Habitat>& habitat_list,
        const double& n_qual_genes,
        const std::vector<double>& preferences,
        const std::vector<double>& quals,
        const std::vector<int>& location,
        std::vector<double>& viab_dist)
{
    const int pop_size = preferences.size();
    for (int i = 0; i < pop_size; ++i)
    {
        /// Viability is the quality effect ^ (missing quality) *
        /// exp( -1/2 * ( character - neutral / selection ) ^ 2 )
        const double temp =
                (preferences[i] - habitat_list[location[i]].get_optimal_preference()) /
                habitat_list[location[i]].get_selection_on_pref();
        viab_dist[i] =
                pow((1 - habitat_list[location[i]].get_selection_on_quality()),
                (1 - (quals[i] / n_qual_genes))) * exp(-0.5 * temp * temp);
    }
}

/// Calculate the viability of an individual given a trait.
void Simulation::crt_male_viability(
        const std::vector<Habitat>& habitat_list,
        const double& n_qual_genes,
        const std::vector<double>& traits,
        const std::vector<double>& quals,
        const std::vector<int>& location,
        std::vector<double>& viab_dist)
{
    const int pop_size = traits.size();
    for (int i = 0; i < pop_size; ++i)
    {
        /// Viability is the quality effect ^ (missing quality) *
        /// exp( -1/2 * ( character - neutral / selection ) ^ 2 )
        const double temp =
                (traits[i] - habitat_list[location[i]].get_optimal_trait()) /
                habitat_list[location[i]].get_selection_on_trt();
        viab_dist[i] =
                pow((1 - habitat_list[location[i]].get_selection_on_quality()),
                (1 - (quals[i] / n_qual_genes))) * exp(-0.5 * temp * temp);
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
    std::uniform_real_distribution<double> mutation_dist(0.0, 1.0);
    for (int i = 0; i < pop_size; ++i)
    {
        /// Chance is proportional to number of good quality genes individual i has.
        if (mutation_dist(generator) < (qual_dec_mu * population[i].get_quality()))
        {
            population[i].mutate_qual_dec(generator);
        }
    }
}

void Simulation::setup_initial_population(
        std::mt19937& generator,
        const Parameters& p,
        std::vector<Individual>& population,
        std::vector<int>& location)
{
    int pop_size = static_cast<int>(p.get_pop_size());
    double migration_rate = static_cast<double>(p.get_migration_rate());
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
    migration(generator, migration_rate, location);
}

void Simulation::setup_output_titles(
        std::ofstream& stats,
        std::ofstream& histograms,
        const Parameters& p,
        const std::vector<Habitat>& habitat_list,
        const std::vector<Individual>& population,
        const std::vector<int>& location,
        const std::vector<int>& location0)
{
    p.print_parameters(stats); /// Print the parameters to the stats file.
    p.print_parameters(histograms); /// Print the parameters to the histograms file.
    int n_habitats = static_cast<int>(habitat_list.size());
    for (int i = 0; i < n_habitats; ++i)
    {
        stats << "\nhabitat," << i << std::endl;
        habitat_list[i].print_habitat(stats); /// Print the habitat parameters to the stats file.
        histograms << "\nhabitat," << i << std::endl;
        /// Print the habitat parameters to the histograms file.
        habitat_list[i].print_habitat(histograms);
    }
    /// Put the column headers on the stats file.
    stats << "habitat_0,,,,,,,,,habitat_1\n"
          << "generation,habitat_pop,mean_pref,mean_trt,mean_qual,pref_variance,"
          << "trt_variance,qual_variance,covariance,correlation,"
          << "generation,habitat_pop,mean_pref,mean_trt,mean_qual,pref_variance,"
          << "trt_variance,qual_variance,covariance,correlation\n0,";
    /// Create an inverted location vector to use for stats collecting.
    /// Collect stats for the population in each location.
    statistics(stats, population, location0);
    stats << "0,";
    statistics(stats, population, location);
    stats << std::endl;
    /// Print the relevant headers to the histogram file.
    setup_histogram_titles(histograms, p);
    histogram(histograms, p, population);
}

/// Individuals migrate between the two populations at random.
// Maybe have the probability based on the individual's phenotype.
void migration(
        std::mt19937& generator,
        const double& migration_rate,
        std::vector<int>& location)
{
    /// Calculate the number of individuals in habitat 1.
    double pop_in_first = static_cast<double>(std::accumulate(
                              std::begin(location),
                              std::end(location),
                              0.0));
    /// Create a probability distribution to use for the migration rates.
    std::uniform_real_distribution<double> migration_dist(0.0, 1.0);
    const int pop_size = static_cast<int>(location.size());
    const double population = static_cast<double>(location.size());
    int count0 = 0;
    int count1 = 0;
    for (int i = 0; i < pop_size; ++i) /// Calculate for each individual
    {
        if (location[i]) /// if they are in habitat 1
        {
            /// given the migration rate and the abundance of individuals in
            /// habitat 1, have a chance of moving to the other habitat.
            if (migration_dist(generator) < 2 * migration_rate * pop_in_first / population)
            {
                location[i] = 0;
                ++count1;
            }
        }
        else /// if they are in habitat 0
        {
            /// given the migration rate and the abundance of individuals in
            /// habitat 0, have a chance of moving to the other habitat.
            if (migration_dist(generator) < 2 * migration_rate * (1 - pop_in_first / population))
            {
                location[i] = 1;
                ++count0;
            }
        }
    }
    std::cout << ',' << pop_in_first << ',' << pop_size - pop_in_first << ',' << count0 << ',' << count1 << std::endl;
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
double variance_calc(
        const std::vector<double>& v,
        const int pop_size)
{
    double vector_size = static_cast<double>(pop_size);
    return (sum(square_vector(v)) - (sum(v)  * sum(v)) / vector_size) / vector_size;
}

/// Calculates the covariance of two vectors.
double covariance_calc(
        const std::vector<double>& v1,
        const std::vector<double>& v2,
        const int pop_size)
{
    /// Check the two vectors are the same size.
    if (v1.size() != v2.size())
    {
        throw std::invalid_argument("Covariance vectors must be equal in size.");
    }
    const double vector_size = static_cast<double>(pop_size);
    std::vector<double> multiplier(v1.size());
    std::transform(std::begin(v1), std::end(v1), std::begin(v2), std::begin(multiplier),
                   std::multiplies<double>());
    return (sum(multiplier) - ((sum(v1) * sum(v2)) / vector_size)) / vector_size;
}
