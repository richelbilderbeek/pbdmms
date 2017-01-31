#include "lyke_individual.h"
#include "lyke_random.h"
#include "lyke_utils.h"
#include "lyke_simulation.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <QFile>
#include <cassert>
#include <stdexcept>
#include "pbd_helper.h"
#include "lyke_parameters.h"

//global variables

//std::vector <Individual*> nextPopulation(popSize, nullptr);

//creates population vectors of individuals
//vector of pointers of type individual
//nullptr: sets the initial state of the individuals of the population at zero.




void recreate_defaultresults_output(const std::string& filename)
{
  QFile f(":/files/lyke_defaultresults.csv");

  assert(f.size());

  if (pbd::is_regular_file(filename))
  {
    pbd::delete_file(filename);
  }
  assert(!pbd::is_regular_file(filename));

  //Copy will fail if filename is already present
  const bool success{
    f.copy(filename.c_str())
  };
  if(!success)
  {
    throw std::runtime_error(":/files/lyke_defaultresults.csv not found");
  }
}

void doStatistics(const std::vector<Individual>& population)
{
  double dSumX = 0.0, dSumSqX = 0.0;
  for (int i = 0; i < static_cast<int>(g_parameters.get_popSize()); ++i)
  {
    const double tmp = population[i].getEcotype();
    dSumX += tmp;
    dSumSqX += tmp * tmp;
  }
 // double dAvg = dSumX / g_parameters.get_popSize(); //calculates population ecotype average
  //double dSdv = sqrt(fabs((dSumSqX / g_parameters.get_popSize()) - dAvg * dAvg));
  //calculates populations ecotype standard deviation
  //std::cout << "Average ecoype:" << " " << dAvg << '\n';
  //std::cout << "Standard deviation:" << " " << dSdv << '\n';
}

void doHistogram(const std::vector<Individual>& population, const int gen, std::ofstream& HistogramFilestream)
{
  std::vector <int> Histogram(14, 0);
  for (int i = 0; i < static_cast<int>(g_parameters.get_popSize()); ++i)
  {
    assert(i >= 0);
    assert(i < static_cast<int>(population.size()));
    double ecotype = population[i].getEcotype();
    int xmin = -4;
    int xmax = 4;
    int bin = static_cast<int>(13 * (ecotype - xmin) / (xmax - xmin));
    if (bin < 0) bin = 0;
    else if (bin > 13) bin = 13;
    ++Histogram[bin];
  }
  HistogramFilestream << gen << ',';
  for (int j = 0; j < 14; ++j)
  {
    HistogramFilestream << Histogram[j] << ',';
  }
  HistogramFilestream << '\n';
}

rnd::discrete_distribution calculates_viability(const std::vector<Individual>& population)
{
  rnd::discrete_distribution viability(g_parameters.get_popSize());
  //vector with viability of each individual and calculates with discrete_distribution the chance
  //of picking an individual depending on the viability.
  for (int i = 0; i < g_parameters.get_popSize(); ++i) {
    //for each individual i, calculates the competition impact of individual j
    for (int j = i + 1; j < g_parameters.get_popSize(); ++j) {
      const double impact_ij = population[i].CalcCompetionIntensity(&population[j]); //Just use pointers because they make you look cool
      viability[i] += impact_ij;
      // viability of i determined by the sum of competition impact
      viability[j] += impact_ij;
      // viability of j is also determined by the sum of competition impact
    }
    const double dz = population[i].getEcotype() / g_parameters.get_sigmaK();
    //normalised distance of ecotype of an individual to optimum
    const double K = (g_parameters.get_popSize() - 1.0) * exp(-0.5 * dz * dz); //carrying capacity
    viability[i] = exp(-g_parameters.get_alpha() * viability[i] / K);
    //viability of individual i (nr of possible offspring),
    //stored in viability(popsize) vector
  }
  return viability;
}


void show_output(const std::vector<Individual>& population, std::ofstream& EcoTypeFilestream, std::ofstream& DefaultresultsFiles) noexcept
{
  for (int i = 0; i < static_cast<int>(g_parameters.get_popSize()); ++i)
  {
    EcoTypeFilestream << ',' << population[i].getEcotype() << ',' << i + 1 << '\n';
   // population[i].print(); //VITAL!
    if (i==0) DefaultresultsFiles<< population[i].getEcotype() << '\n';
  }
}


void replace_current_generation_by_new(std::vector<Individual>& population, std::vector<Individual>& nextPopulation)
{

  assert(population.size() == nextPopulation.size());
  population = nextPopulation;
}

std::vector<int> calc_possible_n_offsprings(rnd::discrete_distribution& viability)
{
  const int n = viability.size();
  std::vector<int> n_offspring(n, 0); //vector for the actual nr of offspring per individual
  for (int i = 0; i < n; ++i)
  {
    ++n_offspring[viability.sample()];
     //depending on the viability, an individual will be picked and reproduces offspring
      //(high viability, higher chance of being picked)
  }
  return n_offspring;
}


std::vector<Individual> create_next_generation(std::vector<int> n_offspring,rnd::discrete_distribution& viability, const std::vector<Individual>& population)
{
  std::vector<Individual> nextPopulation;
  for (int i = 0; i < g_parameters.get_popSize(); ++i)
  {
    if (n_offspring[i]) //if the nr of offspring > 0,
    {
      rnd::discrete_distribution attractiveness(g_parameters.get_popSize());
      //vector with attractiveness of individuals,
      //picks individuals depending on their match (xi,yj)
      for (int j = 0; j < g_parameters.get_popSize(); ++j)
      //calculates the attractiveness of individual i for individual j
      attractiveness[j] = viability[j] * population[i].match(&population[j]); //Pointers are like John Travolta
      //attractiveness depending on the possible nr of offspring (viability)
      //times the 'match' with the other individual
      while (n_offspring[i])
      {
        const int j = attractiveness.sample();
        const std::vector<double> testZ = population[j].getZ();
        assert(testZ.size() != 0);
        assert(i >= 0);
        assert(i < static_cast<int>(population.size()));
        assert(j >= 0);
        assert(j < static_cast<int>(population.size()));


        const Individual& mother = population[i];
        const Individual& father = population[j];
        assert(father.getZ().size() == mother.getZ().size());
        nextPopulation.push_back(Individual(mother, father));
        //next population consisting of the offspring of two individuals (i,j)

        --n_offspring[i];
      }
    }
  }
  // to verify if the size of the next population equals the size of the 'old' population

  //Postcondition
  assert(nextPopulation.size() == population.size());
  return nextPopulation;
}

std::vector<Individual> create_and_log_next_generation(
  const std::vector<Individual>& population,
  std::ofstream& EcoTypeFilestream, std::ofstream& DefaultresultsFiles)
{
  assert(all_individuals_have_the_same_number_of_ecotype_genes(population));

  //calculates viability to test if individuals have the ability to reproduce
  rnd::discrete_distribution viability = calculates_viability(population);

  //produce offspring
  const std::vector<int> n_offspring = calc_possible_n_offsprings(viability);

  //vaibility selection on offspring
  const std::vector<Individual> nextPopulation = create_next_generation(n_offspring, viability, population);
  assert(population.size() == nextPopulation.size());

  show_output(population, EcoTypeFilestream, DefaultresultsFiles); //VITAL! It is a *brilliant* idea not to just show the population, but also write _results_ to a file!

  assert(population.size() == nextPopulation.size());
  return nextPopulation;
}


