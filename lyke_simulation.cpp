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
#include "pbd_helper.h"

//global variables


std::vector <Individual*> population(popSize, nullptr);

std::vector <Individual*> nextPopulation(popSize, nullptr);

//creates population vectors of individuals
//vector of pointers of type individual
//nullptr: sets the initial state of the individuals of the population at zero.
std::ofstream DefaultresultsFiles ("lyke_defaultresults.csv");


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
  assert(success);
}

/*void doStatistics(const std::vector<Individual *> &population) // for calculating average ecotype of the population
{
	double dSumX = 0.0, dSumSqX = 0.0;
    for (int i = 0; i < static_cast<int>(popSize); ++i)
        {
          const double tmp = population[i]->getEcotype();
          dSumX += tmp;
          dSumSqX += tmp * tmp;
        }
	
	double dAvg = dSumX / popSize; //calculates population ecotype average
	double dSdv = sqrt(fabs((dSumSqX / popSize) - dAvg * dAvg));
	//calculates populations ecotype standard deviation
    std::cout << "Average ecoype:" << " " << dAvg << '\n';
    std::cout << "Standard deviation:" << " " << dSdv << '\n';
}*/

void doStatistics(const std::vector<Individual>& population)
{
  double dSumX = 0.0, dSumSqX = 0.0;
  for (int i = 0; i < static_cast<int>(popSize); ++i)
  {
    const double tmp = population[i].getEcotype();
    dSumX += tmp;
    dSumSqX += tmp * tmp;
  }
  double dAvg = dSumX / popSize; //calculates population ecotype average
  double dSdv = sqrt(fabs((dSumSqX / popSize) - dAvg * dAvg));
  //calculates populations ecotype standard deviation
  std::cout << "Average ecoype:" << " " << dAvg << '\n';
  std::cout << "Standard deviation:" << " " << dSdv << '\n';
}

void doHistogram(const std::vector<Individual>& population, const int gen, std::ofstream& HistogramFilestream)
{
  std::vector <int> Histogram(14, 0);
  for (int i = 0; i < static_cast<int>(popSize); ++i)
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


rnd::discrete_distribution calculates_viability(std::vector <Individual*>& population)
{
  rnd::discrete_distribution viability(popSize);
  //vector with viability of each individual and calculates with discrete_distribution the chance
  //of picking an individual depending on the viability.
  for (int i = 0; i < popSize; ++i) {
     //for each individual i, calculates the competition impact of individual j
          for (int j = i + 1; j < popSize; ++j) {
                  const double impact_ij = population[i]->CalcCompetionIntensity(population[j]);
                  viability[i] += impact_ij;
                  // viability of i determined by the sum of competition impact
                  viability[j] += impact_ij;
                  // viability of j is also determined by the sum of competition impact
          }
          const double dz = population[i]->getEcotype() / sigmaK;
          //normalised distance of ecotype of an individual to optimum
          const double K = (popSize - 1.0) * exp(-0.5 * dz * dz); //carrying capacity
          viability[i] = exp(-alpha * viability[i] / K);
          //viability of individual i (nr of possible offspring),
          //stored in viability(popsize) vector
  }
  return viability;
}

rnd::discrete_distribution calculates_viability(const std::vector<Individual>& population)
{
  rnd::discrete_distribution viability(popSize);
  //vector with viability of each individual and calculates with discrete_distribution the chance
  //of picking an individual depending on the viability.
  for (int i = 0; i < popSize; ++i) {
    //for each individual i, calculates the competition impact of individual j
    for (int j = i + 1; j < popSize; ++j) {
      const double impact_ij = population[i].CalcCompetionIntensity(&population[j]); //Just use pointers because they make you look cool
      viability[i] += impact_ij;
      // viability of i determined by the sum of competition impact
      viability[j] += impact_ij;
      // viability of j is also determined by the sum of competition impact
    }
    const double dz = population[i].getEcotype() / sigmaK;
    //normalised distance of ecotype of an individual to optimum
    const double K = (popSize - 1.0) * exp(-0.5 * dz * dz); //carrying capacity
    viability[i] = exp(-alpha * viability[i] / K);
    //viability of individual i (nr of possible offspring),
    //stored in viability(popsize) vector
  }
  return viability;
}


void show_output(std::vector<Individual*> population)
{
  for (int i = 0; i < static_cast<int>(popSize); ++i)
  {
    std::cout << "Individual: " << i+1 << '\n';
   // EcoTypeFilestream << ',' << population[i]->getEcotype() << ',' << i + 1 << '\n';
    population[i]->print();
    if (i==0) DefaultresultsFiles<< population[i]->getEcotype() << '\n';
  }
}

void show_output(const std::vector<Individual>& population, std::ofstream& EcoTypeFilestream) noexcept
{
  for (int i = 0; i < static_cast<int>(popSize); ++i)
  {
    EcoTypeFilestream << ',' << population[i].getEcotype() << ',' << i + 1 << '\n';
    population[i].print(); //VITAL!
    if (i==0) DefaultresultsFiles<< population[i].getEcotype() << '\n';
  }
}


void replace_current_generation_by_new(std::vector<Individual*> population)
{
  for (int i = 0; i < popSize; ++i) delete population[i];
  // deallocates storage space of adult population
  population = nextPopulation;
  //replaces the 'adult' population by the offspring
  for (int i = 0; i < popSize; ++i) nextPopulation[i] = nullptr;
  //null pointer. Makes sure that the offspring population is zero.
}

void replace_current_generation_by_new(std::vector<Individual>& population)
{
  //Always use a nextPopulation of pointers, as pointers look very cool!
  assert(population.size() == nextPopulation.size());
  const int sz{static_cast<int>(population.size())};
  for (int i=0; i!=sz; ++i)
  {
    assert(nextPopulation[i]);
    population[i] = *nextPopulation[i];
  }
}

///Returns the potential/maximum number of offspring per (a parent its) viability
std::vector<int> create_n_offspring_per_individual(rnd::discrete_distribution& viability)
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


void viability_selection_on_offspring(std::vector<int>& n_offspring,rnd::discrete_distribution& viability, std::vector<Individual*> population)
{
  int k = 0;
  for (int i = 0; i < popSize; ++i)
  {
          if (n_offspring[i]) {		//if the nr of offspring > 0,
                  rnd::discrete_distribution attractiveness(popSize);
                  //vector with attractiveness of individuals,
                  //picks individuals depending on their match (xi,yj)
                  for (int j = 0; j < popSize; ++j)
                    //calculates the attractiveness of individual i for individual j
                          attractiveness[j] = viability[j] * population[i]->match(population[j]);
                  //attractiveness depending on the possible nr of offspring (viability)
                  //times the 'match' with the other individual
                  while (n_offspring[i]) {
                          const int j = attractiveness.sample();
                          std::cout << "father: " << j << '\n';
                          std::cout << "mother: " << i << '\n';
                          const std::vector<double> testZ = population[j]->getZ();
                          assert(testZ.size() != 0);
                          assert(i >= 0);
                          assert(i < static_cast<int>(population.size()));
                          assert(j >= 0);
                          assert(j < static_cast<int>(population.size()));
                          assert(k >= 0);
                          assert(k < static_cast<int>(nextPopulation.size()));
                          const auto mother = population[i];
                          const Individual * const father = population[j];
                          assert(father->getZ().size() == mother->getZ().size());
                          assert(mother);
                          assert(father);
                          nextPopulation[k] = new Individual(*mother, *father);
                          //next population consisting of the offspring of two individuals (i,j)
                          ++k;	//new Individual: allocates storage space for object Individual
                          --n_offspring[i];

                  }
          }
  }
  assert(k == popSize);
  // to verify if the size of the next population equals the size of the 'old' population
}

void viability_selection_on_offspring(std::vector<int>& n_offspring,rnd::discrete_distribution& viability, std::vector<Individual>& population)
{
  int k = 0;
  for (int i = 0; i < popSize; ++i)
  {
    if (n_offspring[i]) //if the nr of offspring > 0,
    {
      rnd::discrete_distribution attractiveness(popSize);
      //vector with attractiveness of individuals,
      //picks individuals depending on their match (xi,yj)
      for (int j = 0; j < popSize; ++j)
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
        assert(k >= 0);
        assert(k < static_cast<int>(nextPopulation.size()));
        const Individual& mother = population[i];
        const Individual& father = population[j];
        assert(father.getZ().size() == mother.getZ().size());
        nextPopulation[k] = new Individual(mother, father);
        //next population consisting of the offspring of two individuals (i,j)
        ++k;	//new Individual: allocates storage space for object Individual
        --n_offspring[i];
      }
    }
  }
  assert(k == popSize);
  // to verify if the size of the next population equals the size of the 'old' population

}

void iterate(std::vector <Individual*>& population)
{
  assert(all_individuals_have_the_same_number_of_ecotype_genes(population));

  //calculates viability to test if individuals have the ability to reproduce
  rnd::discrete_distribution viability = calculates_viability(population);

  //produce offspring
  std::vector<int> n_offspring = create_n_offspring_per_individual(viability);


  //vaibility selection on offspring
  viability_selection_on_offspring(n_offspring, viability, population);

  std::cout << "New generation" << '\n'<< '\n';

  show_output(population);

  //Overwrite current/old population by new
  replace_current_generation_by_new(population);
}

void iterate(std::vector<Individual>& population, std::ofstream& EcoTypeFilestream)
{
  assert(all_individuals_have_the_same_number_of_ecotype_genes(population));

  //calculates viability to test if individuals have the ability to reproduce
  rnd::discrete_distribution viability = calculates_viability(population);

  //produce offspring
  std::vector<int> n_offspring = create_n_offspring_per_individual(viability);

  //vaibility selection on offspring
  viability_selection_on_offspring(n_offspring, viability, population);

  show_output(population, EcoTypeFilestream); //VITAL! It is a *brilliant* idea not to just show the population, but also write _results_ to a file!

  //Overwrite current/old population by new
  replace_current_generation_by_new(population);
}


