#ifndef KEWE_PARAMETERS_H
#define KEWE_PARAMETERS_H

#include <string>
#include <vector>

typedef long unsigned int bigint;

namespace kewe {

struct simulation_parameters
{
  bigint endtime = 10; // End simulation at this generation

  int Nx = 2;         // Number of X alleles
  int Np = 2;         // Number of P alleles
  int Nq = 2;         // Number of Q alleles

  double x0 = 0.5;    // initial x gene
  double p0 = 0.5;    // initial p gene
  double q0 = 0.5;    // initial q gene

  double se = 0.1;    // specificity of mate choice ecological type
  double sm = 0.1;    // specificity of mate choice mating type

  double sc = 0.3;    // unction RJCB: Strength of competition
  double sk = 1.2;    // width of ecological resource distribution

  double sq = 1.0;    // strength of viability selection on male mating type
  double sv = 0.02;   // width distribution mutation sizes

  double c = 0.0005;  // intensity competition

  double at = 0.05;    // attractivity threshold

  int seed = 123;                                 // Seed for RNG
  int haploid = 1;                                // 1 == run simulation haploid
  int diploid = 0;                                // 1 == run simulation diploid
  int popsize = 10;                // Initial population size

};

struct output_parameters
{
  int outputfreq = 10;                            // output frequency
  int histw = 50;                                 // Number of steps in the histograms
  double histbinx = 0.1;                          // stepsize of histogram x
  double histbinp = 0.1;                          // stepsize of histogram p
  double histbinq = 0.1;                          // stepsize of histogram q
  std::string outputfilename = "defaultresults";
  std::string ltt_plot_filename = "defaultlttplot";
  bool is_silent;
};

struct parameters
{
  parameters() : m_sim_parameters{}, m_output_parameters{} {}
  simulation_parameters m_sim_parameters;
  output_parameters m_output_parameters;
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
parameters create_test_parameters_haploid_1() noexcept;

///Use different number of loci
parameters create_test_parameters_haploid_2() noexcept;

///Close-to-simplest parameters for testing purposes
parameters create_test_parameters_diploid_1() noexcept;

///Use different number of loci
parameters create_test_parameters_diploid_2() noexcept;

///The parameters of figure 3 used in Van Doorn & Weissing 2001
parameters create_parameters_article_figure_3() noexcept;

///Read parameters from a file, throws if file is absent or incorrect
parameters read_parameters(const std::string& filename);

} //~namespace kewe

#endif // KEWE_PARAMETERS_H
