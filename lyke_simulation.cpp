#include "lyke_individual.h"
#include "lyke_random.h"
#include "lyke_utils.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <QFile>
#include <cassert>

std::vector <Individual*> population(popSize, nullptr), nextPopulation(popSize, nullptr); //creates population vectors of individuals
//vector of pointers of type individual
//nullptr: sets the initial state of the individuals of the population at zero.
std::ofstream EcoTypeFilestream ("ecotype.csv"); //opens excel file
std::ofstream HistogramFilestream("Histogram.csv");//opens excel file
std::ofstream DefaultresultsFilestream ("lyke_defaultresults.csv");
//std::ofstream SubstitutionFilestream("substitutions.csv"); //opens excel file

void recreate_defaultresults_output(const std::string& filename)
{
    QFile f(":/files/lyke_defaultresults.csv");
    assert(f.size());
    f.copy(filename.c_str());
}


void doStatistics() // for calculating average ecotype of the population
{
	double dSumX = 0.0, dSumSqX = 0.0;
    for (int i = 0; i < static_cast<int>(popSize); ++i)
	{
		const double tmp = population[i]->getEcotype();
		dSumX += tmp;
		dSumSqX += tmp * tmp;
	}
	
	double dAvg = dSumX / popSize; //calculates population ecotype average
	double dSdv = sqrt(fabs((dSumSqX / popSize) - dAvg * dAvg)); //calculates populations ecotype standard deviation
    std::cout << "Average ecoype:" << " " << dAvg << '\n';
    std::cout << "Standard deviation:" << " " << dSdv << std::endl;
	//EcoTypeFilestream << dAvg << ',' << dSdv << std::endl;
}

void doHistogram(int gen)//for making a histogram of the ecotypes
{
	std::vector <int> Histogram(14, 0);
        for (int i = 0; i < static_cast<int>(popSize); ++i)
	{
			double ecotype = population[i]->getEcotype();
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
		} HistogramFilestream << std::endl;
}

/*void doSubstitutions(std::vector<double>&TempsubstitutionsXnonsynonymous, std::vector<double>&TempsubstitutionsXsynonymous, std::vector<double>&TempsubstitutionsYnonsynonymous, std::vector<double>&TempsubstitutionsYsynonymous) //for calculating substitutions
{
	std::vector <double>substitutionsXnonsynonymous(L / 2, 0); //different vectors to store frequencies of indv of population
	std::vector <double>substitutionsXsynonymous(L / 2, 0);
	std::vector <double>substitutionsYnonsynonymous(L / 2, 0);
	std::vector <double>substitutionsYsynonymous(L / 2, 0);
	SubstitutionFilestream << "Individual:" << " " << "nonsynonymous x:" << ',' << "synonymous x:" << ',' << "nonsynonymous y:" << ',' << "synonymous y:" << std::endl;
	for (size_t i = 0u; i < popSize; ++i)
	{
		SubstitutionFilestream << i << ',';
		std::bitset<L> tmp1 = population[i]->getX();
		std::bitset<L> tmp2 = population[i]->getY();
		for (size_t j = 0u; j < L / 2; ++j)
		{
			if(tmp1[2 * j + 1]) 
				substitutionsXnonsynonymous[j]++;
			if (tmp1[2 * j])
				substitutionsXsynonymous[j]++;
		}
		for (size_t j = 0u; j < L / 2; ++j)
		{
			if (tmp2[2 * j + 1])
				substitutionsYnonsynonymous[j]++;
			if (tmp2[2 * j])
				substitutionsYsynonymous[j]++;
		}
	}

	for (size_t i = 0u; i < (L/2); ++i)
	{
		((substitutionsXnonsynonymous[i] / popSize) - TempsubstitutionsXnonsynonymous[i]);

	}
	double DnX;
	for (size_t i = 0u; i < (L / 2); ++i)
	{
		DnX += std::abs (substitutionsXnonsynonymous[i]);
	}
	DnX / (L / 2);
	SubstitutionFilestream << "Dn X" << ','<< DnX << std::endl;
	
	for(size_t i = 0u; i < (L / 2); ++i)
	{
		((substitutionsXsynonymous[i] / popSize) - TempsubstitutionsXsynonymous[i]);
	}
	double DsX;
	for (size_t i = 0u; i < (L / 2); ++i)
	{
		DsX += std::abs(substitutionsXsynonymous[i]);
	}
	DsX / (L / 2);
	SubstitutionFilestream << "Ds X" << ','<< DsX << std::endl;

	for (size_t i = 0u; i < (L / 2); ++i)
	{
		((substitutionsYnonsynonymous[i] / popSize) - TempsubstitutionsYnonsynonymous[i]);

	}
	double DnY;
	for (size_t i = 0u; i < (L / 2); ++i)
	{
		DnY += std::abs(substitutionsYnonsynonymous[i]);
	}
	DnY / (L / 2);
	SubstitutionFilestream << "Dn Y" <<','<< DnY << std::endl;

	for (size_t i = 0u; i < (L / 2); ++i)
	{
		((substitutionsYsynonymous[i] / popSize) - TempsubstitutionsYsynonymous[i]);

	}
	double DsY;
	for (size_t i = 0u; i < (L / 2); ++i)
	{
		DsY += std::abs(substitutionsYsynonymous[i]);
	}
	DsY / (L / 2);
	SubstitutionFilestream << "Ds Y" << ','<< DsY << std::endl;

	TempsubstitutionsXnonsynonymous = substitutionsXnonsynonymous;
	TempsubstitutionsXsynonymous = substitutionsXsynonymous;
	TempsubstitutionsYnonsynonymous = substitutionsYnonsynonymous;
	TempsubstitutionsYsynonymous = substitutionsYsynonymous;
}*/


void iterate()
{
	//calculates viability to test if individuals have the ability to reproduce
	rnd::discrete_distribution viability(popSize); //vector with viability of each individual and calculates with discrete_distribution the chance of picking an individual depending on the viability.
	for (size_t i = 0u; i < popSize; ++i) {			//for each individual i, calculates the competition impact of individual j
		for (size_t j = i + 1u; j < popSize; ++j) {
			const double impact_ij = population[i]->compete(population[j]);
			viability[i] += impact_ij; // viability of i determined by the sum of competition impact
			viability[j] += impact_ij; // viability of j is also determined by the sum of competition impact 
		}
		const double dz = population[i]->getEcotype() / sigmaK; //normalised distance of ecotype of an individual to optimum
		const double K = (popSize - 1.0) * exp(-0.5 * dz * dz); //carrying capacity 
		viability[i] = exp(-alpha * viability[i] / K); //viability of individual i (nr of possible offspring), stored in viability(popsize) vector 
	}

	//produce offspring
	std::vector<int> offspring(popSize, 0); //vector for the actual nr of offspring per individual
	for (size_t i = 0u; i < popSize; ++i)  
		++offspring[viability.sample()]; //depending on the viability, an individual will be picked and reproduces offspring (high viability, higher chance of being picked)
	size_t k = 0u;						
	for (size_t i = 0u; i < popSize; ++i) 
		if (offspring[i]) {					//if the nr of offspring > 0, 
			rnd::discrete_distribution attractiveness(popSize); //vector with attractiveness of individuals, picks individuals depending on their match (xi,yj)
			for (size_t j = 0u; j < popSize; ++j) //calculates the attractiveness of individual i for individual j
				attractiveness[j] = viability[j] * population[i]->match(population[j]);//attractiveness depending on the possible nr of offspring (viability) times the 'match' with the other individual
			while (offspring[i]) {
				const int j = attractiveness.sample();
				nextPopulation[k] = new Individual(population[i], population[j]); //next population consisting of the offspring of two individuals (i,j)
				++k;															//new Individual: allocates storage space for object Individual
				--offspring[i];
			}
		}
	verify(k == popSize); // to verify if the size of the next population equals the size of the 'old' population
    std::cout << "New generation" << '\n'<< std::endl;
	//EcoTypeFilestream << "Individual" << "," << "Ecotype" << ','<< "Generation"<<  "\n" ; //output to csv.file
    for (int i = 0; i < static_cast<int>(popSize); ++i)
	{
    std::cout << "Individual: " << i+1 << '\n';
		EcoTypeFilestream << ',' << population[i]->getEcotype() << ',' << i + 1 << std::endl;
        population[i]->print();
        if (i==0)
            DefaultresultsFilestream<< population[i]->getEcotype() << std::endl;
	}


	for (size_t i = 0u; i < popSize; ++i) delete population[i]; // deallocates storage space of adult population
	population = nextPopulation;								//replaces the 'adult' population by the offspring
	for (size_t i = 0u; i < popSize; ++i) nextPopulation[i] = nullptr; //null pointer. Makes sure that the offspring population is zero.
}



int main()
{
    rnd::set_seed(42);
	Individual::init();
	for (size_t i = 0u; i < popSize; ++i) population[i] = new Individual;//allocates storage space
	echo("simulation started");
    recreate_defaultresults_output("defaultresults");
	//std::vector <double>TempsubstitutionsXnonsynonymous((L / 2), 0); //Temporary vectors to store frequencies of indv of population
	//std::vector <double>TempsubstitutionsXsynonymous((L / 2), 0);
	//std::vector <double>TempsubstitutionsYnonsynonymous((L / 2), 0);
	//std::vector <double>TempsubstitutionsYsynonymous((L / 2), 0);
	EcoTypeFilestream << "Generation" << "," << "Ecotype" << "," << "Individual" << "\n"; //output to csv.file
	HistogramFilestream << "Time,1,2,3,4,5,6,7,8,9,10,11,12,13,14" << std::endl;
    for (int i = 0; i < static_cast<int>(simulationruns); ++i)  //number of generations
	{
		EcoTypeFilestream << 1 + i;
		iterate(); // updates population
        std::cout << " Generation:" << i << " "; //output
		//EcoTypeFilestream << "Generation" << ',' << "Average ecotype" << ',' << "Standard deviation" << std::endl;
		//EcoTypeFilestream << 1 + i;
		doStatistics();
		doHistogram(i+1);
		//doSubstitutions(TempsubstitutionsXnonsynonymous, TempsubstitutionsXsynonymous, TempsubstitutionsYnonsynonymous, TempsubstitutionsYsynonymous);
	}
		
	
	EcoTypeFilestream.close(); //closes excel file
	//SubstitutionFilestream.close(); //closes excel file
	HistogramFilestream.close();
    DefaultresultsFilestream.close();
	
	
	for (size_t i = 0u; i < popSize; ++i) delete population[i];


	wait_for_return();
	return 0;
}
