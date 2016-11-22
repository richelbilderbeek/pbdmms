#include <iostream>
#include "lyke_individual.h"
#include "lyke_random.h"
#include <cmath>
#include <cassert>
#include <fstream>

template <class T>
bool all_not_nllptr(const std::vector<T*>& v)
{
  for (const auto i: v) if (!i) return false;
  return true;
}

std::bitset<L> get_mask()
{
  std::bitset<L> m;
  for (int i = 0; i < L; i+=2)//sets every two bits (even) to 1
  {
    m.set(i);
  }
  return m;
}


Individual::Individual() //Object Individual() of class Individual
  : x{}, y{}, z{}, ecotype{0.0}
{
	for (int i = 0; i < L; ++i)
	{
		if(rnd::uniform() < 0.5) x.set(i);
		//uniform distribution with a likelihood of 0.5 that bits of x are set to 1.
		if (rnd::uniform() < 0.5) y.set(i); 
	}
	z = std::vector<double>(nGeneEco, rnd::normal(0.0, 1.0));
	//Random numbers between 0.0 and 1.0 by normal distribution
	develop(); //calculates phenotype from the ecological character z
	assert (z.size()== nGeneEco);
}

Individual::Individual(
  const Individual& mother,
  const Individual& father
) : x{}, y{}, z{}, ecotype{0.0}
    //Creation of new Individual by copying two existing Individuals
  //ERROR! Father and mother have different z.size()
{
	std::cout<< "size mother" << mother.z.size() << '\n';
	std::cout<< "size father" << father.z.size() << '\n';
	assert(mother.z == mother.getZ());
	assert(father.z == father.getZ());
	assert(mother.z.size() == father.z.size());
	x = rnd::uniform() < 0.5 ? mother.x : father.x;
	// likelihood of 0.5 to have the x from the mother/father Individual
	y = rnd::uniform() < 0.5 ? mother.y : father.y;
	// likelihood of 0.5 to have the y from the mother/father Individual
	for (int i = 0; i < nGeneEco; ++i)
	{
	  assert(i >= 0);
	  assert(i < static_cast<int>(mother.z.size()));
	  assert(i < static_cast<int>(father.z.size()));
	  z.push_back(rnd::uniform() < 0.5 ? mother.z[i] : father.z[i]);
	}
	std::cout<< "z is:"<< z.size()<< '\n';
	//Generates a mix of the "ecological genes", from two Individuals
	mutate(); // Flips a bit in the bitstrings + the ecological character z
	develop(); //calculates phenotype from the ecological character z
	
	assert(z.size() == father.z.size());
	assert(z.size() == mother.z.size());
}

bool all_individuals_have_the_same_number_of_ecotype_genes(
  const std::vector<Individual*>& population
)
{
  assert(all_not_nllptr(population));
  if (population.size() < 2) return true;
  assert(population.back());
  const auto n = population.back()->getZ().size();
  for (const auto i: population)
  {
    assert(i);
    if (i->getZ().size() != n) return false;
  }
  return true;
}

bool all_individuals_have_the_same_number_of_ecotype_genes(
  const std::vector<Individual>& population
)
{
  if (population.size() < 2) return true;
  const auto n = population.back().getZ().size();
  for (const auto i: population)
  {
    if (i.getZ().size() != n) return false;
  }
  return true;
}

double calculate_attraction(const Individual& individual, const Individual& other)
{
  const std::bitset<L> temp = (get_mask() & individual.getX()) ^ (get_mask() & other.getY());
  //compares the x and y string of individuals, stores 0 for match and 1 for mismatch
  return exp(- beta * temp.count());// counts every the nr of 1 in the string
}

void Individual::mutate()
//With the chance of having a mutation: flips a bit in the bitstrings
{
	int mutation;
	if (rnd::uniform() < (mu*L)) 
	{
		mutation = rnd::integer(L);
		x[mutation].flip();
		// flips a bit of the bitstring at the position of the value ascribed to mutation
	}
	if (rnd::uniform() < (mu*L))
	{
		mutation = rnd::integer(L);
		y[mutation].flip();
		// flips a bit of the bitstring at the position of the value ascribed to mutation
	}
	if (rnd::uniform() < (mu*nGeneEco))
	{
		mutation = rnd::integer(nGeneEco);
		// with a normal distibution, increases the z element.
		z[mutation] += rnd::normal(0.0, sigmaMut);
	}
	assert (z.size()== nGeneEco);
}

void Individual::develop()
//The phenotype based on the genotype of the ecological character
{
	double sum = 0.0;
	for (int i = 0; i < nGeneEco; ++i)
		sum += z[i];
	ecotype = sum / nGeneEco;
}
double Individual::CalcCompetionIntensity(Individual const * const other) const
// calculates the competition impact between individual and others
{
	double dz = (ecotype - other->ecotype) / sigmac;
	// (Q: dz is the competition intensity?)
	return exp(-0.5* dz * dz);
}

double Individual::match(Individual const * const other) const
//calculates the probability of mating between individual
//and all the other individuals from the population
{
	std::bitset<L> temp = (get_mask() & x) ^ (get_mask() & other->y);
	//compares the x and y string of individuals, stores 0 for match and 1 for mismatch
	return exp(- beta * temp.count());// counts every the nr of 1 in the string
}

void Individual::ugly()
{
  y.reset();
  x.reset();
}


void Individual::print() const //output
{
	std::cout << "x = " << ",";
	std::cout << x << '\n';
	std::cout << "y = " << ",";
	std::cout << y << '\n';
	std::cout << "z = " << ",";
	for (int i = 0; i < nGeneEco; ++i) std::cout << z[i] << ' ';
	std::cout << '\n';
	std::cout << "ecotype = " << ",";
	std::cout << ecotype << '\n' << '\n';
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

std::ostream& operator<<(std::ostream& os, const Individual& /* individual */) noexcept
{
  os << "STUB";
  return os;
}
