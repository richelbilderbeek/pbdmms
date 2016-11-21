#ifndef KEWE_PARAMETERS_H
#define KEWE_PARAMETERS_H

#include <string>
#include <vector>

typedef long unsigned int bigint;

struct kewe_simulation_parameters
{
  bigint endtime = 10; // End simulation at this generation

  int Nx = 2; // Number of X alleles
  int Np = 2; // Number of P alleles
  int Nq = 2; // Number of Q alleles

  double x0 = 0.5; // initial x gene
  double p0 = 0.5; // initial p gene
  double q0 = 0.5; // initial q gene

  double sc = 0.3; //?? TODO Look up sc
  double se = 0.1; //??      Look up se
  double sk = 1.2; //??      Look up sk
  double sq = 1.0; //??      Look up sq
  double sm = 0.1; //??      Look up sm
  double sv = 0.02;//??      Look up sv

  double c = 0.0005; // constant to stabalize populationsize

  double eta = 1.0; //?? TODO Look up eta
  double b = 4; //number of babies

  int seed = 123;                                 // Seed for RNG
  int haploid = 1;                                // 1 == run simulation haploid
  int diploid = 0;                                // 1 == run simulation diploid
  long unsigned int popsize = 10;                // Initial population size

};

struct kewe_output_parameters
{
  int outputfreq = 10;                            // output frequency
  int histw = 50;                                 // Number of steps in the histograms
  double histbinx = 0.1;                          // stepsize of histogram x
  double histbinp = 0.1;                          // stepsize of histogram p
  double histbinq = 0.1;                          // stepsize of histogram q
  std::string outputfilename = "defaultresults";
};

struct kewe_parameters
{
  kewe_parameters() : sim_parameters{}, output_parameters{} {}
  kewe_simulation_parameters sim_parameters;
  kewe_output_parameters output_parameters;
};

bool is_regular_file(const std::string& filename) noexcept;

std::vector<std::string> file_to_vector(const std::string& filename);
std::vector<std::string> seperate_string(
  const std::string& input,
  const char seperator);
double str_to_double(const std::string& s);

///Creates a parameter file that can be used for testing with the name 'filename'
///Will throw if file cannot be created
void create_test_parameter_file1(const std::string& filename);
void create_test_parameter_file2(const std::string& filename);
void create_test_parameter_file3(const std::string& filename);
void create_test_parameter_file4(const std::string& filename);
void create_test_parameter_file5(const std::string& filename);
void create_test_parameter_file6(const std::string& filename);

///Close-to-simplest parameters for testing purposes
kewe_parameters create_test_parameters() noexcept;

///Read parameters from a file, throws if file is absent or incorrect
kewe_parameters read_parameters(const std::string& filename);


#endif // KEWE_PARAMETERS_H

