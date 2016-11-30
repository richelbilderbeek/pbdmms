#include <iostream>
#include <vector>
#include <fstream>
#include <exception>
#include <cstdlib>
#include <cmath>

const int		nvar = 4;
const double	tEnd = 150.0;
const double	dt = 0.5;
const double	dtsav = 1;

const double	C = 1.0;
const double	pi = 3.1415926;


void rhs(const double &t, std::vector<double> &x, std::vector<double> &dVdt)
{
	double stim;
	if ( t < 100 && t > 50)
		stim = 10.0 * (1 - cos((2 * pi * t) / 150.0));
	else
		stim = 0;
	double n4 = x[3] * x[3] * x[3] * x[3];
	double m3 = x[1] * x[1] * x[1];
	dVdt[0] = (1.0 / C) * m3 * x[2] * 120.0 * (x[0] + 115.0) + 36.0 * n4 * (x[0] - 12.0) + 0.3 * (x[0] + 10.5989) + stim;;
	dVdt[1] = 0.1 * (1 - x[1]) * (x[0] + 25.0) / (exp(0.1 * x[0] + 0.25) - 4.0 * x[1] * exp(x[0] / 18.0));
	dVdt[2] = 0.07 * (1 - x[2]) * exp(x[0] / 20.0) - x[2] / (exp(0.1 * x[0] + 3.0) + 1);
	dVdt[3] = 0.01 * (1 - x[3]) * (x[0] + 10.0) / (exp(0.1 * x[0] + 1) - 1) - 0.125 * x[3] * exp(x[0] / 80.0);
}

void rungeKuttaStepper(double &t, std::vector<double> &x, const double &h)
{
	std::vector<double> dVdt1(nvar);
	rhs( t , x, dVdt1);

	std::vector<double> xtemp(nvar);
	for (int i = 0; i < nvar; ++i)
		xtemp[i] = x[i] + 0.5 * h * dVdt1[i];
	std::vector<double> dVdt2(nvar);
	rhs(t + 0.5 * h, xtemp, dVdt2);

	for (int i = 0; i < nvar; ++i)
		xtemp[i] = x[i] + 0.5 * h * dVdt2[i];
	std::vector<double> dVdt3(nvar);
	rhs(t + 0.5 * h, xtemp, dVdt3);

	for (int i = 0; i < nvar; ++i)
		xtemp[i] = x[i] + h * dVdt3[i];
	std::vector<double> dVdt4(nvar);
	rhs(t + h, xtemp, dVdt4);

	for (int i = 0; i < nvar; ++i)
		x[i] += h * (dVdt1[i] + 2.0 * dVdt2[i] + 2.0 * dVdt3[i] + dVdt4[i]) / 6;
	t += h;
}

int main()
{


	try {
		std::vector<double> x(nvar);
		x[0] = 0.0;
		x[1] = 0.05;
		x[2] = 0.59;
		x[3] = 0.32;

		std::ofstream ofs("numintRK50.csv");
		if (!ofs.is_open())
			throw std::runtime_error("unable to open file. \n");

		for (double t = 0.0, tsav = 0.0; t < 50; ) {
			rungeKuttaStepper(t, x, dt);

			if (t > tsav) {
				std::cout << "time:" << t << ' ' <<"Voltage: " << x[0] << "gating variable m: " << x[1] <<"h: " << x[2] << "n: " << x[3] <<'\n';
				ofs << t << ',' << x[0] << ',' << x[1] << ',' << x[2] << ',' << x[3] << '\n';						
				tsav += dtsav;
			}
		}
		ofs.close();

		std::ofstream ofs2("numintRK100.csv");
		if (!ofs.is_open())
			throw std::runtime_error("unable to open file. \n");

		for (double t = 50.0, tsav = 0.0; t < 100; ) {
			rungeKuttaStepper(t, x, dt);

			if (t > tsav) {
				std::cout << "time:" << t << ' ' << "Voltage: " << x[0] << "gating variable m: " << x[1] << "h: " << x[2] << "n: " << x[3] << '\n';
				ofs << t << ',' << x[0] << ',' << x[1] << ',' << x[2] << ',' << x[3] << '\n';
				tsav += dtsav;
			}
		}
		ofs2.close();

		std::ofstream ofs3("numintRK150.csv");
		if (!ofs.is_open())
			throw std::runtime_error("unable to open file. \n");

		for (double t = 100.0, tsav = 0.0; t < 150; ) {
			rungeKuttaStepper(t, x, dt);

			if (t > tsav) {
				std::cout << "time:" << t << ' ' << "Voltage: " << x[0] << "gating variable m: " << x[1] << "h: " << x[2] << "n: " << x[3] << '\n';
				ofs << t << ',' << x[0] << ',' << x[1] << ',' << x[2] << ',' << x[3] << '\n';
				tsav += dtsav;
			}
		}
		ofs3.close();
	}
	catch (std::exception &error) {
		std::cerr << "error:" << error.what();
		exit(EXIT_FAILURE);
	}
	catch (...) {
		std::cerr << "unable to continue due to unknown error." << std::endl;
		exit(EXIT_FAILURE);
	}

	return 0;
}