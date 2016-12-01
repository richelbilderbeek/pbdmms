#include <iostream>
#include <vector>
#include <exception>
#include <cmath>
#include <random>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <cassert>

int main()
{
    const double d{8.0};
    const int    n = 13;
    const double h = d / 13;
    const double t = 298.0;
    const double f = 2.0;
    const double kon = 200;
    const double koff = 50.0;
    const double kb{1.3806485 * pow(10.0 , -23.0 )};
    assert(kb > 0.0);
    const double tEnd{1.0};


    try
    {

        std::vector<int> filaments(n , 20);				//initialising microtubule protofilaments
        double barrier = 0.0;							//initialising barrier

		std::vector<double> length(n);
		std::vector<double> rateon(n);
		std::vector<double> dx(n);

		std::chrono::high_resolution_clock::time_point tp =
			std::chrono::high_resolution_clock::now();
		unsigned seed = static_cast<unsigned>(tp.time_since_epoch().count());

		std::mt19937_64 rng;
		rng.seed(seed);

        for(double tm = 0.0; tm < tEnd;){
            for (int i = 0; i < n; ++i) {
               assert(length.size() == filaments.size() );
               assert(i >= 0);
               assert(!filaments.empty());
               assert(i < static_cast<int>(filaments.size()));
               assert(i < static_cast<int>(length.size()));
               length[i] = filaments[i] * d + h * i;

               if (length[i] > barrier)
                    barrier = length[i];					//longest filament is where the barrier is
			}
			double sumrateson = 0.0;
			double sumrates = 0.0;
			for (int j = 0; j < n; ++j) {
                assert(j >= 0);
                assert(j < static_cast<int>(dx.size()));
                assert(j < static_cast<int>(length.size()));
                dx[j] = (length[j] + d) - barrier;
                assert(kb * t != 0.0);
                assert(j < static_cast<int>(rateon.size()));
                assert(200 * exp(-20) > 0);
                assert(!dx.empty());
                rateon[j] = kon * exp(-(f * dx[j]) / (kb * t * pow(10 , 21)));
                assert(dx.size() == n);
                assert(rateon.size() == n);
                assert(rateon[j] > 0 );
				sumrateson += rateon[j];
				sumrates += rateon[j] + koff;
			}
            assert (sumrates > 0);
            assert (sumrateson > 0);
			if (sumrates <= 0.0 || sumrateson <= 0.0)
				throw std::runtime_error("unable to draw waiting time.\n");
			

			std::vector<double> allRates(2);
			allRates[0] = sumrateson;
			allRates[1] = sumrates - sumrateson;

			std::exponential_distribution<double> waitingTime(sumrates);			//draw waiting time
            double dt = waitingTime(rng);
            tm += dt;

			std::discrete_distribution<int> onOrOff(allRates.begin(), allRates.end());		//draw event on or off
			int k = onOrOff(rng);

			std::discrete_distribution<int> addFilament(rateon.begin(), rateon.end());		//on is discrete distribution
			std::uniform_int_distribution<int> removeFilament(0, n);						//off is uniform distribution

			if (k == 0) {
                int a = addFilament(rng);

				filaments[a] += 1;
			}
			if (k == 1) {
				int a = removeFilament(rng);

				filaments[a] -= 1;
			}
        }
		for (int m = 0; m < n; ++m)
			std::cout << filaments[m] << "  " << length[m] << '\n';
	}
	catch (std::exception &error) {
		std::cerr << "error:" << error.what();
		exit(EXIT_FAILURE);
	}
	
    return 0;
}

