#include <iostream>
#include "lyke_individual.h"
#include "lyke_random.h"
#include <cmath>
#include <cassert>
#include <fstream>
#include "lyke_parameters.h"

template <class T>
bool all_not_nllptr(const std::vector<T*>& v)
{
  for (const auto i: v) if (!i) return false;
  return true;
}

boost::dynamic_bitset<> get_mask(const int L)
{
  boost::dynamic_bitset<> m(L);
  for (int i = 0; i < L; i+=2)//sets every two bits (even) to 1
  {
    m.set(i);
  }
  return m;
}

Individual::Individual() //Object Individual() of class Individual
  : x(g_parameters.get_L()), y(g_parameters.get_L()), z{}, ecotype{0.0}
{
	for (int i = 0; i < g_parameters.get_L(); ++i)
	{
		if(rnd::uniform() < 0.5) x.set(i);
		//uniform distribution with a likelihood of 0.5 that bits of x are set to 1.
		if (rnd::uniform() < 0.5) y.set(i); 
	}
	z = std::vector<double>(g_parameters.get_nGeneEco(), rnd::normal(0.0, 1.0));
	//Random numbers between 0.0 and 1.0 by normal distribution
	develop(); //calculates phenotype from the ecological character z
	assert (static_cast<int>(z.size()) == g_parameters.get_nGeneEco());
}

Individual::Individual(
  const Individual& mother,
  const Individual& father
) : x(g_parameters.get_L()), y(g_parameters.get_L()), z{}, ecotype{0.0}
    //Creation of new Individual by copying two existing Individuals

{

  assert(mother.z == mother.getZ());
  assert(father.z == father.getZ());
  assert(mother.z.size() == father.z.size());
  x = rnd::uniform() < 0.5 ? mother.x : father.x;
  // likelihood of 0.5 to have the x from the mother/father Individual
  y = rnd::uniform() < 0.5 ? mother.y : father.y;
  // likelihood of 0.5 to have the y from the mother/father Individual
  for (int i = 0; i < g_parameters.get_nGeneEco(); ++i)
  {
    assert(i >= 0);
    assert(i < static_cast<int>(mother.z.size()));
    assert(i < static_cast<int>(father.z.size()));
    z.push_back(rnd::uniform() < 0.5 ? mother.z[i] : father.z[i]);
  }

  //Generates a mix of the "ecological genes", from two Individuals
  mutate(); // Flips a bit in the bitstrings + the ecological character z
  develop(); //calculates phenotype from the ecological character z

  assert(z.size() == father.z.size());
  assert(z.size() == mother.z.size());
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

double calculate_fertilization_efficiency(const Individual& individual, const Individual& other)
{
  assert(individual.getX().size() == other.getY().size());
  return calculate_fertilization_efficiency(
    individual.getX(), //Egg
    other.getY(),  //Sperm
    g_parameters.get_beta(),
    get_mask(individual.getX().size())
  );
}

double calculate_fertilization_efficiency(
  const boost::dynamic_bitset<>& egg_loci,
  const boost::dynamic_bitset<>& sperm_loci,
  const double decay_rate_per_mismatch,
  const boost::dynamic_bitset<>& ignore_loci_mask
)
{
  assert(ignore_loci_mask.size() == sperm_loci.size());
  assert(egg_loci.size() == sperm_loci.size());
  assert(decay_rate_per_mismatch >= 0.0);
  //Mask removes the synonymous (that is, unimportant) loci
  // Egg   : 00000000
  // Sperm : 00001111
  // Mask  : 01010101
  // Result: 01000000 it's a match if egg[i] == sperm[i] AND mask[i] == 0 (is not ignored)
  const boost::dynamic_bitset<> matching_loci
    = (ignore_loci_mask & egg_loci) ^ (ignore_loci_mask & sperm_loci);
  const int n_matching_loci{static_cast<int>(matching_loci.count())};

  return std::exp(-decay_rate_per_mismatch * n_matching_loci);
}


void Individual::mutate()
//With the chance of having a mutation: flips a bit in the bitstrings
{
	int mutation;
	if (rnd::uniform() < (g_parameters.get_mu()*g_parameters.get_L()))
	{
		mutation = rnd::integer(g_parameters.get_L());
		x[mutation].flip();
		// flips a bit of the bitstring at the position of the value ascribed to mutation
	}
	if (rnd::uniform() < (g_parameters.get_mu()*g_parameters.get_L()))
	{
		mutation = rnd::integer(g_parameters.get_L());
		y[mutation].flip();
		// flips a bit of the bitstring at the position of the value ascribed to mutation
	}
	if (rnd::uniform() < (g_parameters.get_mu()*g_parameters.get_nGeneEco()))
	{
		mutation = rnd::integer(g_parameters.get_nGeneEco());
		// with a normal distibution, increases the z element.
		z[mutation] += rnd::normal(0.0, g_parameters.get_sigmaMut());
	}
	assert (static_cast<int>(z.size())== g_parameters.get_nGeneEco());
}

void Individual::develop()
//The phenotype based on the genotype of the ecological character
{
	double sum = 0.0;
	for (int i = 0; i < g_parameters.get_nGeneEco(); ++i)
		sum += z[i];
	ecotype = sum / g_parameters.get_nGeneEco();
}
double Individual::CalcCompetionIntensity(Individual const * const other) const
// calculates the competition impact between individual and others
{
	double dz = (ecotype - other->ecotype) / g_parameters.get_sigmac();
	// (Q: dz is the competition intensity?)
	return exp(-0.5* dz * dz);
}

double Individual::match(Individual const * const other) const
//calculates the probability of mating between individual
//and all the other individuals from the population
{
	boost::dynamic_bitset<> temp = (get_mask(g_parameters.get_L()) & x) ^ (get_mask(g_parameters.get_L()) & other->y);
	//compares the x and y string of individuals, stores 0 for match and 1 for mismatch
	return exp(- g_parameters.get_beta() * temp.count());// counts every the nr of 1 in the string
}

void Individual::ugly()
{
  y.reset();
  x.reset();
}


/*void Individual::print() const //output
{
	std::cout << "x = " << ",";
	std::cout << x << '\n';
	std::cout << "y = " << ",";
	std::cout << y << '\n';
	std::cout << "z = " << ",";
	for (int i = 0; i < g_parameters.get_nGeneEco(); ++i) std::cout << z[i] << ' ';
	std::cout << '\n';
	std::cout << "ecotype = " << ",";
	std::cout << ecotype << '\n' << '\n';
}*/

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
