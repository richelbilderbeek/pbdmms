#include <chrono>
#include <sstream>
#include <iomanip>
#include "lyke_random.h"
#include <stdexcept>

namespace rnd {
    std::mt19937 rng;
        const double BINOMIALCOST = 7.0;
        //time cost of drawing a binomial() deviate relative to drawing a uniform() deviate

    void set_seed(const unsigned seed)
    {
        //std::ostringstream oss;
        //oss << "random seed set to " << seed;
        //echo(oss.str());
        rng.seed(seed);
    }
    	
    long integer(const long &n)
    {
        typedef std::uniform_int_distribution<long> distribution;
        static long k = n;
        static distribution distr(0, k - 1l);
        if (k != n) {
            k = n;
            distr = distribution(0, k - 1l);
        }
        return distr(rng);
    }


	double uniform()
	{
        static std::uniform_real_distribution<double> distr(0.0, 1.0);
        return distr(rng);
	}
    
    double normal(const double &mean, const double &stdev)
	{
        typedef std::normal_distribution<double> distribution;
        static double z1 = mean, z2 = stdev > 0.0 ? stdev : 1.0;
        static distribution distr(z1, z2);
        if(stdev > 0.0) {
            if (z1 != mean || z2 != stdev) {
                z1 = mean;
                z2 = stdev;
                distr = distribution(z1, z2);
            }
        }
        else if (stdev == 0.0) return mean;
        else throw std::invalid_argument("argument out of range");
        return distr(rng);
	}
    
    discrete_distribution::discrete_distribution(const int &sz)
      : n(sz), is_accumulated{false},pdf(std::vector<double>(sz, 0.0)),cdf(std::vector<double>(sz))
    {
        is_accumulated = false;
    }
    
    void discrete_distribution::accumulate()
    {
        double sum = 0.0;
                for (std::vector<double>::iterator itp = pdf.begin(),
                     itc = cdf.begin(); itp != pdf.end(); ++itp, ++itc)
            *itc = sum += *itp;
        is_accumulated = true;
    }
    
        int discrete_distribution::sample()
	//samples from the cdf using bisection
    {
        if (!is_accumulated) accumulate();
		int jmin = -1, jmax = n - 1;
        const double f = uniform() * cdf.back();
		while(jmax - jmin > 1)
        {
            const int jmid = (jmax + jmin) / 2;
            if(f > cdf[jmid]) jmin = jmid;
            else jmax = jmid;
        }
        return jmax;
    }
}
