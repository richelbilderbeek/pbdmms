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

Simulation::Simulation() {
}

void Simulation::run(
        std::mt19937& generator,
        const Parameters& p)
{
    const int pop_size = static_cast<int>(p.get_pop_size());
    std::vector<Individual> population(pop_size, Individual(p));
    for (int i = 0; i < pop_size; ++i)
    {
        population[i].init_population(generator, p);
    }
    std::ofstream stats("jaan_stats.csv");
    p.print_parameters(stats);
//    std::ofstream histograms("jaan_histograms.csv");
    stats << "generation,mean_pref,mean_trt,mean_qual,pref_variance,"
          << "trt_variance,qual_variance,covariance,correlation\n0,";
    statistics(stats, population);
//    histograms << "generation,0" << std::endl;
//            histogram(histograms, p, population);
    for (int g = 0; g < p.get_max_generations(); ++g)
    {
        std::cout << "generation " << g << std::endl;
        if (((g + 1) % 100) == 0)
        {
            stats << g << ',';
            statistics(stats, population);
//            histograms << "generation," << g << std::endl;
//            histogram(histograms, p, population);
        }
        population = create_next_gen(generator, p, population);
    }
    stats.close();
//    histograms.close();
}

// Calculate the mean and variance of pref and trt and qual and covariance of pref and trt.
void Simulation::statistics(
        std::ofstream& stats,
        std::vector<Individual>& population)
{
    const std::vector<double> prefs = collect_prefs(population);
    const double mean_pref = mean(prefs);
    const double pref_variance = variance_calc(prefs);
    const std::vector<double> trts = collect_trts(population);
    const double mean_trt = mean(trts);
    const double trt_variance = variance_calc(trts);
    const std::vector<double> quals = collect_quals(population);
    const double covariance = covariance_calc(prefs, trts);
    const double correlation = covariance / (pow(pref_variance, 0.5) * pow(trt_variance, 0.5));
    const double mean_qual = mean(quals);
    const double qual_variance = variance_calc(quals);
    std::cout << "mean_pref " << mean_pref
              << " mean_trt " << mean_trt
              << " mean_qual " << mean_qual
              << " pref_variance " << pref_variance
              << " trt_variance " << trt_variance
              << " qual_variance " << qual_variance
              << " covariance " << covariance
              << " correlation " << correlation << std::endl;
    stats << mean_pref << ','
          << mean_trt << ','
          << mean_qual << ','
          << pref_variance << ','
          << trt_variance << ','
          << qual_variance << ','
          << covariance << ','
          << correlation << std::endl;
}

/* Create two histograms, one of preferences in the population and one of male traits.
 * Possible values for the histogram run from all -1 to all +1 so the size is the difference
 * between the two, i.e. the all -1, all +1 plus the all 0 state.
 */
void Simulation::histogram(
        std::ofstream& histograms,
        const Parameters& p,
        std::vector<Individual>& population)
{
    const int pop_size = static_cast<int>(p.get_pop_size());
    const double n_pref_genes = static_cast<double>(p.get_n_pref_genes());
    const double n_trt_genes = static_cast<double>(p.get_n_trt_genes());
    const double scale_preference = static_cast<double>(p.get_scale_preference());
    const double scale_trait = static_cast<double>(p.get_scale_trait());
    std::vector<double> pref_hist(n_pref_genes + 1);
    std::vector<double> trt_hist(n_trt_genes + 1);
    for (int i = 0; i < pop_size; ++i)
    {
        create_histogram(n_pref_genes, population[i].get_preference(),
                         scale_preference, pref_hist);
        create_histogram(n_trt_genes, population[i].get_trait(), scale_trait, trt_hist);
        // NOT SUMMING TO 1000 ON TRAIT BUT IS ON PREFERENCE.
    }
    const char pref_title1[] = "Preference_Value";
    const char pref_title2[] = "Preference_Histogram";
    output_histogram(histograms, n_pref_genes, pref_title1, pref_title2, pref_hist);
    const char trt_title1[] = "Trait_Value";
    const char trt_title2[] = "Trait_Histogram";
    output_histogram(histograms, n_trt_genes, trt_title1, trt_title2, trt_hist);
}

std::vector<Individual> Simulation::create_next_gen(
        std::mt19937& generator,
        const Parameters& p,
        std::vector<Individual>& population)
{
    const int pop_size = static_cast<int>(p.get_pop_size());
    std::vector<Individual> offspring;
    offspring.reserve(pop_size);
    for (int i = 0; i < pop_size; ++i)
    {
        const int mother = pick_mother(generator, p, population);
        const int father = pick_father(generator, p, population, mother);
        Individual child(generator, p, population[mother], population[father]);
        offspring.push_back(child);
    }
    return offspring;
}

/* This function calculates which individual will become a mother using the
 * cumulative probabilities and a random number. It sets mother to the position
 * the successful female is at in the vector.
 */
int Simulation::pick_mother(
        std::mt19937& generator,
        const Parameters& p,
        std::vector<Individual>& population)
{
    const int pop_size = static_cast<int>(p.get_pop_size());
    std::vector<double> female_viab_dist(pop_size);
    std::vector<double> prefs = collect_prefs(population);
    std::vector<double> quals = collect_quals(population);
    crt_viability(prefs, quals, p.get_optimal_preference(), p.get_value_of_preference(),
                  p.get_quality_viab(), female_viab_dist);
    std::discrete_distribution<int> mother_distribution(female_viab_dist.begin(),
                                                        female_viab_dist.end());
    return mother_distribution(generator);
}

// separate out a function to call for attractivity calculation.
int Simulation::pick_father(
        std::mt19937& generator,
        const Parameters& p,
        std::vector<Individual>& population,
        const int& mother)
{
    const int pop_size = static_cast<int>(p.get_pop_size());
    const double quality_effect = static_cast<double>(p.get_quality_effect());
    std::vector<double> male_viab_dist(pop_size);
    std::vector<double> trts = collect_trts(population);
    std::vector<double> quals = collect_quals(population);
    crt_viability(trts, quals, p.get_optimal_trait(), p.get_value_of_trait(),
                  p.get_quality_viab(), male_viab_dist);
    std::vector<double> attractivity(pop_size);
    for (int i = 0; i < pop_size; ++i)
    {
        attractivity[i] = male_viab_dist[i] *
                exp(population[mother].get_preference() * population[i].get_trait() *
                    // turned off individual quality component to see that preference and trait are proportional to the quality effect
                    quality_effect);// * quals[i]);
    }
    std::discrete_distribution<int> father_distribution(attractivity.begin(), attractivity.end());
    return father_distribution(generator);

    /// REMOVE ONCE SEXUAL SELECTION IS ON.
//    return mother;
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
