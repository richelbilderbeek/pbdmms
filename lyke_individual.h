/* This program investigates the divergence of sex-related genes
by sexual selection and sympatric speciation
Lyke Stuurman 20/10/16 */


#ifndef individual_h
#define individual_h

#include <boost/dynamic_bitset.hpp>
#include <vector>

class Individual
{
public:
	Individual();
	// constructor (function to create objects)
	Individual(const Individual& mother, const Individual& father);
	//copy constructor (function to create offsping from two individuals)
	//void print() const;
	//function to print the Individual-object variables (x, y, z)
	double getEcotype() const noexcept { return ecotype; }
	double CalcCompetionIntensity(Individual const * const) const;
	//competition function
	double match(Individual const * const) const;
	//function for the match between individuals
	//function for creating a 'mask' string
	boost::dynamic_bitset<> getX() const noexcept { return x; }
	// function to get the egg protein gene
	const std::vector<double>& getZ() const noexcept { return z; }
	boost::dynamic_bitset<> getY() const noexcept { return y; }
	// function to get the sperm protein gene
	void ugly ();
	//function to make an individual ugly- for simulation test

private:
	boost::dynamic_bitset<> x, y;
	//bitstring of x and y with length L
	std::vector<double> z;
	//vector for the ecological component
	double ecotype;
	//phenotype
	void develop();
	//calculating phenotype from the ecological genes
	void mutate();
	//mutation function
	//static boost::dynamic_bitset<> isNonsynSite;
	//Mask string: there is only one copy of this, it is the same for every individual

	friend bool operator==(const Individual& lhs, const Individual& rhs) noexcept;
};

boost::dynamic_bitset<> get_mask(const int L);

///Checks if all individuals have the same number of ecotype genes
bool all_individuals_have_the_same_number_of_ecotype_genes(
  const std::vector<Individual>& population
);

///Calculate the attraction between two individuals, will return a value from
///0.0 (no attraction) to 1.0 (maximum attraction)
double calculate_attraction(const Individual& boy, const Individual& girl);

///Create an individual that is of the opposite sexual attractiveness, that
///is, the individual that 'i' thinks is very ugly
Individual create_opposite(const Individual& i) noexcept;

bool operator==(const Individual& lhs, const Individual& rhs) noexcept;
bool operator!=(const Individual& lhs, const Individual& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const Individual& individual) noexcept;

#endif
