/* Final weekassignment. This program investigates the divergence of sex-related genes by sexual selection and sympatric speciation
Lyke Stuurman 20/10/16 */


#ifndef individual_h
#define individual_h

#include <bitset>
#include <vector>


const size_t simulationruns = 5000u;	//nr of simulation runs/ generations
const size_t L = 20u;					//length of bitstrings (size_t= unsigned integer type)
const size_t nGeneEco = 10u;			//nr of ecological genes
const double mu = 0.001;				//mutation rate
const double sigmaMut = 0.2;			//standard deviation
const size_t popSize = 3500u;			//population size
const double sigmac = 1.0;				//width of competition function
const double sigmaK = 5.0;				//width of carrying capacity function
const double alpha = 1.0;				//strength of selection on ecotype
const double beta = 0.1;				//decay rate of matching probability per mismatch

class Individual
{
public:
	Individual(); // constructor (function to create objects)
	Individual(Individual const * const, Individual const * const); //copy constructor (function to create offsping from two individuals)
	void print() const; //function to print the Individual-object variables (x, y, z)
	double getEcotype() const { return ecotype; }
	double compete(Individual const * const); //competition function
	double match(Individual const * const);	//function for the match between individuals
	static void init();						//function for creating a 'mask' string
	std::bitset<L> getX() const { return x; } // function to get the egg protein gene
	std::bitset<L> getY() const { return y; } // function to get the sperm protein gene


private:
	std::bitset<L> x, y;	//bitstring of x and y with length L
	std::vector<double> z;	//vector for the ecological component
	double ecotype;			//phenotype
	void develop();			//calculating phenotype from the ecological genes
	void mutate();			//mutation function
	static std::bitset<L> isNonsynSite;	//Mask string: there is only one copy of this, it is the same for every individual
};



#endif
