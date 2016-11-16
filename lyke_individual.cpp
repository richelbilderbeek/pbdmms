#include <iostream>
#include "lyke_individual.h"
#include "lyke_random.h"
#include <cmath>
#include <fstream>

std::bitset<L> Individual::isNonsynSite;	//global bitstring


void Individual::init()
{
	isNonsynSite.reset();					//sets every bit to 0
	for (size_t i = 0u; i < L; i+=2)		//sets every two bits (even) to 1
		isNonsynSite.set(i);
}

Individual::Individual() //Object Individual() of class Individual
  : x{}, y{}, z{}, ecotype{0.0}
{
	for (size_t i = 0u; i < L; ++i)
	{
		if(rnd::uniform() < 0.5) x.set(i); //uniform distribution with a likelihood of 0.5 that bits of x are set to 1.
		if (rnd::uniform() < 0.5) y.set(i); 
	}
	z = std::vector<double>(nGeneEco, rnd::normal(0.0, 1.0)); //Random numbers between 0.0 and 1.0 by normal distribution
	develop(); //calculates phenotype from the ecological character z

}

Individual::Individual(Individual const * const mother, Individual const * const father)//Creation of new Individual by copying two existing Individuals 
    : x{}, y{}, z{}, ecotype{0.0}
{
	x = rnd::uniform() < 0.5 ? mother->x : father->x; // likelihood of 0.5 to have the x from the mother/father Individual
	y = rnd::uniform() < 0.5 ? mother->y : father->y; // likelihood of 0.5 to have the y from the mother/father Individual
	for (size_t i = 0u; i < nGeneEco; ++i)
		z.push_back(rnd::uniform() < 0.5 ? mother->z[i] : father->z[i]); //Generates a mix of the "ecological genes", from two Individuals
	mutate(); // Flips a bit in the bitstrings + the ecological character z
	develop(); //calculates phenotype from the ecological character z
	
}


void Individual::mutate() //With the chance of having a mutation: flips a bit in the bitstrings
{
	int mutation;
	if (rnd::uniform() < (mu*L)) 
	{
		mutation = rnd::integer(L);
		x[mutation].flip(); // flips a bit of the bitstring at the position of the value ascribed to mutation
	}
	if (rnd::uniform() < (mu*L))
	{
		mutation = rnd::integer(L);
		y[mutation].flip(); // flips a bit of the bitstring at the position of the value ascribed to mutation
	}
	if (rnd::uniform() < (mu*nGeneEco))
	{
		mutation = rnd::integer(nGeneEco); // with a normal distibution, increases the z element.
		z[mutation] += rnd::normal(0.0, sigmaMut);
	}
}

void Individual::develop() //The phenotype based on the genotype of the ecological character
{
	double sum = 0.0;
	for (size_t i = 0u; i < nGeneEco; ++i)
		sum += z[i];
	ecotype = sum / nGeneEco;
}
// (Q: How does it check for every other individual in the population?)
double Individual::CalcCompetionIntensity(Individual const * const other) const // (Q: const other is a const pointer to a const of type individual?) calculates the competition impact between individual and others
{
	double dz = (ecotype - other->ecotype) / sigmac;  // (Q: dz is the competition intensity?)
	return exp(-0.5* dz * dz);
}

double Individual::match(Individual const * const other) const //calculates the probability of mating between individual and all the other individuals from the population
{
	std::bitset<L> temp = (isNonsynSite & x) ^ (isNonsynSite & other->y);//compares the x and y string of individuals, stores 0 for match and 1 for mismatch
	return exp(- beta * temp.count());					// counts every the nr of 1 in the string
}


void Individual::print() const		//output
{
	std::cout << "x = " << ",";
	std::cout << x << '\n';
	std::cout << "y = " << ",";
	std::cout << y << '\n';
	std::cout << "z = " << ",";
	for (size_t i = 0u; i < nGeneEco; ++i) std::cout << z[i] << ' ';
	std::cout << '\n';
	std::cout << "ecotype = " << ",";
	std::cout << ecotype << '\n' << std::endl;
}

bool operator==(const Individual& lhs, const Individual& rhs) noexcept
{
  return lhs.x == rhs.x
    &&  lhs.y == rhs.y
    &&  lhs.z == rhs.z
    &&  lhs.ecotype == rhs.ecotype
  ;
}


bool operator!=(const Individual& lhs, const Individual& rhs) noexcept
{
  return !(lhs == rhs);
}
