#include <iostream>
#include <vector>
#include <exception>
#include <cmath>
#include <random>
#include <chrono>
#include <cstdlib>
#include <fstream>

const double		d = 8.0;
const int			n = 13;
const double		h = d / 13;
const double		t = 298.0;
const double		f = 2.0;
const double		kon = 200;
const double		koff = 50;
const double		kb = 1.3806485;
const double		tEnd = 10;

int main()
{
	try {

		std::vector<int> filaments(n, 5);				//initialising microtubule protofilaments
		double barrier = 0.0;							//initialising barrier

		std::vector<double> length(n);
		std::vector<double> rateon(n);
		std::vector<double> dx(n);

		std::chrono::high_resolution_clock::time_point tp =
			std::chrono::high_resolution_clock::now();
		unsigned seed = static_cast<unsigned>(tp.time_since_epoch().count());

		std::mt19937_64 rng;
		rng.seed(seed);


		for (int s = 0; s < 50; ++s) {
			for (int i = 0; i < n; ++i) {
				length[i] = filaments[i] * d + h * i;

				if (length[i] > barrier)
					barrier = length[i];					//longest filament is where the barrier is
			}
			double sumrateson = 0.0;
			double sumrates = 0.0;
			for (int j = 0; j < n; ++j) {
				dx[j] = barrier - length[j];
				rateon[j] = kon * exp(-(f * dx[j]) / (kb * t));
				sumrateson += rateon[j];
				sumrates += rateon[j] + koff;
			}
			if (sumrates <= 0.0 || sumrateson <= 0.0)
				throw std::runtime_error("unable to draw waiting time.\n");
			

			std::vector<double> allRates(2);
			allRates[0] = sumrateson;
			allRates[1] = sumrates - sumrateson;

			std::exponential_distribution<double> waitingTime(sumrates);			//draw waiting time
			double dt = waitingTime(rng);
			//tm += dt;

			std::discrete_distribution<int> onOrOff(allRates.begin(), allRates.end());		//draw event on or off
			int k = onOrOff(rng);

			std::discrete_distribution<int> addFilament(rateon.begin(), rateon.end());		//on is discrete distribution
			std::uniform_int_distribution<int> removeFilament(0, n);						//off is uniform distribution

			int a;
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

