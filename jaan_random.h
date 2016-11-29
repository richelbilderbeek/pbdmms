/*========================================================================================================
                                                   random.h
==========================================================================================================
 
 Routines for the generation of pseudo-random numbers
 
 C++-code accompanying:
 
        xxx
 
 Written by:
        G. Sander van Doorn
        Groningen Institute for Evolutionary Life Sciences (Gelifes)
        University of Groningen
        the Netherlands
 
 Program version
        dd/mm/yyyy	: xxx
 
 =======================================================================================================*/

#ifndef random_h
#define random_h

#include <random>
#include <vector>
#include "utils.h"

namespace rnd
{
    class discrete_distribution {
    public:
        discrete_distribution(const int&);
        double& operator[](const int &i) { is_accumulated = false; return pdf[i]; }
        double operator()(const int &i) const { return pdf[i]; }
        int sample();
        std::vector<int> sample(int);
        int size() const { return n;}
    private:
        void accumulate();
        const int n;
        bool is_accumulated;
        std::vector<double> pdf, cdf;
    };
    
    void set_seed();
	void set_seed(const unsigned int&);
    int integer(const int&);
	bool bernoulli(const double& = 0.5);
	int binomial(const int&, const double& = 0.5);
	int poisson(const double& = 1.0);
	double uniform();
    double normal(const double& = 0.0, const double& = 1.0);
    double exponential(const double& = 1.0);
    
    extern std::mt19937 rng;
}

#endif //#ifndef random_h