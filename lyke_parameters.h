#ifndef LYKE_PARAMETERS_H
#define LYKE_PARAMETERS_H

class lyke_parameters
{
public:
  lyke_parameters();

  const int    simulationruns = 10;//nr of simulation runs/ generations
  const int    L = 20;		 //length of bitstrings
  const int    nGeneEco = 10;	 //nr of ecological genes
  const double mu = 0.001;	 //mutation rate
  const double sigmaMut = 0.2;	 //standard deviation
  const int    popSize = 10;	 //population size
  const double sigmac = 1.0;	 //width of competition function
  const double sigmaK = 5.0;	 //width of carrying capacity function
  const double alpha = 1.0;	 //strength of selection on ecotype
  const double beta = 0.1;	 //decay rate of matching probability per mismatch
};

#endif // LYKE_PARAMETERS_H
