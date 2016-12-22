#ifndef KEWE_PARAMETERS_H
#define KEWE_PARAMETERS_H

#include "kewe_output_parameters.h"
#include "kewe_simulation_parameters.h"

namespace kewe {

struct parameters
{
  parameters() : m_sim_parameters{}, m_output_parameters{} {}
  simulation_parameters m_sim_parameters;
  output_parameters m_output_parameters;
};

///?
void create_header(const parameters& parameters);

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

///The parameters used in profiling
parameters create_profiling_parameters() noexcept;

///Read parameters from a file, throws if file is absent or incorrect
parameters read_parameters(const std::string& filename);

} //~namespace kewe

#endif // KEWE_PARAMETERS_H
