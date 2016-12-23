#include <cassert>
#include <fstream>
#include <vector>
#include <algorithm>
#include <boost/algorithm/string/split.hpp>
#include "kewe_parameters.h"
#include "kewe_ses.h"
#include "kewe_helper.h"

void kewe::create_header(const parameters& parameters)
{
  std::ofstream out(parameters.m_output_parameters.outputfilename);
  const int histw = parameters.m_output_parameters.histw;
  out<<"generation,popsize,rhoxp,rhoxq,rhopq,sx,sp,sq,";

  for(int k=0;k<histw;k++)
      out<<","<<(k-histw/2)*parameters.m_output_parameters.histbinx;
  for(int k=0;k<histw;k++)
      out<<","<<(k-histw/2)*parameters.m_output_parameters.histbinp;
  for(int k=0;k<histw;k++)
      out<<","<<(k-histw/2)*parameters.m_output_parameters.histbinq;
  out<< '\n';
}

kewe::parameters kewe::read_parameters(const std::string& filename) //!OCLINT Readable, no easier way to read parameters.
{
  const auto lines = file_to_vector(filename);

  parameters parameters;

  for (const std::string& line: lines)
  {
    std::vector<std::string> v{seperate_string(line, ' ')};
    if (v[0] == "alleles")
      {
        v.erase(v.begin());
        for (int i = 0; i < static_cast<int>(v.size()); ++i)
        {
          switch(i)
          {
            case 0: parameters.m_sim_parameters.Nx = std::stod(v[0]); break;
            case 1: parameters.m_sim_parameters.Np = std::stod(v[1]); break;
            case 2: parameters.m_sim_parameters.Nq = std::stod(v[2]); break;
            default: throw std::invalid_argument("Too many parameters after \"alleles\"");
          }
        }
      }
    else if(v[0] == "type0")
      {
        v.erase(v.begin());
        for (int i = 0; i < static_cast<int>(v.size()); ++i)
        {
          switch(i)
          {
            case 0: parameters.m_sim_parameters.x0 = std::stod(v[0]); break;
            case 1: parameters.m_sim_parameters.p0 = std::stod(v[1]); break;
            case 2: parameters.m_sim_parameters.q0 = std::stod(v[2]); break;
            default: throw std::invalid_argument("Too many parameters after \"type0\"");
          }
        }
      }
    else if(v[0] == "histbin")
      {
        v.erase(v.begin());
        for (int i = 0; i < static_cast<int>(v.size()); ++i)
        {
          switch(i)
          {
            case 0: parameters.m_output_parameters.histbinx = std::stod(v[0]); break;
            case 1: parameters.m_output_parameters.histbinp = std::stod(v[1]); break;
            case 2: parameters.m_output_parameters.histbinq = std::stod(v[2]); break;
            default: throw std::invalid_argument("Too many parameters after \"histbin\"");
          }
        }
      }
    else if(v[0] == "seed"){parameters.m_sim_parameters.seed = std::stod(v[1]);}
    else if(v[0] == "pop0"){parameters.m_sim_parameters.popsize = std::stod(v[1]);}
    else if(v[0] == "end"){parameters.m_sim_parameters.set_end_time(std::stod(v[1]));}
    else if(v[0] == "sc"){parameters.m_sim_parameters.sc = std::stod(v[1]);}
    else if(v[0] == "se"){parameters.m_sim_parameters.se = std::stod(v[1]);}
    else if(v[0] == "sk")
    {
      parameters.m_sim_parameters.set_eco_res_distribution_width(std::stod(v[1]));
    }
    else if(v[0] == "c"){parameters.m_sim_parameters.c = std::stod(v[1]);}
    else if(v[0] == "sm"){parameters.m_sim_parameters.sm = std::stod(v[1]);}
    else if(v[0] == "sv"){parameters.m_sim_parameters.set_mut_distr_width(std::stod(v[1]));}
    else if(v[0] == "sq"){parameters.m_sim_parameters.sq = std::stod(v[1]);}
    else if(v[0] == "at"){parameters.m_sim_parameters.at = std::stod(v[1]);}
    else if(v[0] == "output")
      {

        v.erase(v.begin());
        assert(v.size() >= 1);
        parameters.m_output_parameters.outputfreq = std::stod(v[0]);
        if(v.size() >= 2)
          parameters.m_output_parameters.outputfilename = v[1];
            }
    else if(v[0] == "ploidy")
      {
        if(std::stod(v[1]) == 1)
        {
          parameters.m_sim_parameters.set_ploidy(ploidy::diploid);
        }
        else if(std::stod(v[1]) == 0)
        {
          parameters.m_sim_parameters.set_ploidy(ploidy::haploid);
        }
      }
  }



  return parameters;
}

void kewe::create_test_parameter_file1(const std::string& filename)
{
  std::ofstream f(filename.c_str());
  f << "alleles 2 2 2 2\n"
    << "histbin 0.1 0.1 0.1\n"
    << "seed 123\n"
    << "pop0 100\n"
    << "type0 0.5 0.5 0.5\n"
    << "end 10\n"
    << "sc 0.3\n"
    << "se 0.1\n"
    << "sk 1.2\n"
    << "c 0.0005\n"
    << "sm 0.1\n"
    << "sv 0.02\n"
    << "sq 1.0\n"
    << "eta 1.0\n"
    << "b 4.0\n"
    << "output 10 defaultresults\n"
    << "ploidy 0\n";
}

void kewe::create_test_parameter_file2(const std::string& filename)
{
  std::ofstream f(filename.c_str());
  f << "alleles 2 2 2\n"
    << "histbin 0.1 0.1 0.1 0.1\n"
    << "seed 123\n"
    << "pop0 100\n"
    << "type0 0.5 0.5 0.5\n"
    << "end 10\n"
    << "sc 0.3\n"
    << "se 0.1\n"
    << "sk 1.2\n"
    << "c 0.0005\n"
    << "sm 0.1\n"
    << "sv 0.02\n"
    << "sq 1.0\n"
    << "eta 1.0\n"
    << "b 4.0\n"
    << "output 10 defaultresults\n"
    << "ploidy 0\n";
}

void kewe::create_test_parameter_file3(const std::string& filename)
{
  std::ofstream f(filename.c_str());
  f << "alleles 2 2 2\n"
    << "histbin 0.1 0.1 0.1\n"
    << "seed 123\n"
    << "pop0 100\n"
    << "type0 0.5 0.5 0.5 0.5\n"
    << "end 10\n"
    << "sc 0.3\n"
    << "se 0.1\n"
    << "sk 1.2\n"
    << "c 0.0005\n"
    << "sm 0.1\n"
    << "sv 0.02\n"
    << "sq 1.0\n"
    << "eta 1.0\n"
    << "b 4.0\n"
    << "output 10 defaultresults\n"
    << "ploidy 0\n";
}

void kewe::create_test_parameter_file4(const std::string& filename)
{
  std::ofstream f(filename.c_str());
  f << "alleles 2 2 2\n"
    << "histbin 0.1 0.1 0.1\n"
    << "seed 123\n"
    << "pop0 100\n"
    << "type0 0.5 0.5 0.5\n"
    << "end 10\n"
    << "sc 0.3\n"
    << "se 0.1\n"
    << "sk 1.2\n"
    << "c 0.0005\n"
    << "sm 0.1\n"
    << "sv 0.02\n"
    << "sq 1.0\n"
    << "eta 1.0\n"
    << "b 4.0\n"
    << "output 10 defaultresults\n"
    << "ploidy 1\n";
}

void kewe::create_test_parameter_file5(const std::string& filename)
{
  std::ofstream f(filename.c_str());
  f << "alleles 2 2 2\n"
    << "histbin 0.1 0.1 0.1\n"
    << "seed 123\n"
    << "pop0 100\n"
    << "type0 0.5 0.5 0.5\n"
    << "end 10\n"
    << "sc 0.3\n"
    << "se 0.1\n"
    << "sk 1.2\n"
    << "c 0.0005\n"
    << "sm 0.1\n"
    << "sv 0.02\n"
    << "sq 1.0\n"
    << "eta 1.0\n"
    << "b 4.0\n"
    << "output 10 defaultresults\n"
    << "ploidy 0\n";
}

void kewe::create_test_parameter_file6(const std::string& filename)
{
  std::ofstream f(filename.c_str());
  f << "alleles 2 2 2\n"
    << "histbin 0.1 0.1 0.1\n"
    << "seed 123\n"
    << "pop0 100\n"
    << "type0 0.5 0.5 0.5\n"
    << "end 10\n"
    << "sc 0.3\n"
    << "se 0.1\n"
    << "sk 1.2\n"
    << "c 0.0005\n"
    << "sm 0.1\n"
    << "sv 0.02\n"
    << "sq 1.0\n"
    << "eta 1.0\n"
    << "b 4.0\n"
    << "output 10 defaultresults\n"
    << "ploidy 1\n";
}

kewe::parameters kewe::create_test_parameters_haploid_1() noexcept
{
  parameters p;
  p.m_sim_parameters = create_test_sim_parameters_haploid_1();
  p.m_output_parameters.outputfreq = 1; //Every generation
  p.m_output_parameters.is_silent = true;
  return p;
}

kewe::parameters kewe::create_test_parameters_haploid_2() noexcept
{
  parameters p;
  p.m_sim_parameters = create_test_sim_parameters_haploid_2();
  p.m_output_parameters.outputfreq = 1; //Every generation
  p.m_output_parameters.is_silent = true;
  return p;
}

kewe::parameters kewe::create_test_parameters_diploid_1() noexcept
{
  parameters p;
  p.m_sim_parameters = create_test_sim_parameters_diploid_1();
  p.m_output_parameters.outputfreq = 1; //Every generation
  p.m_output_parameters.is_silent = true;
  return p;
}

kewe::parameters kewe::create_test_parameters_diploid_2() noexcept
{
  parameters p;
  p.m_sim_parameters = create_test_sim_parameters_diploid_2();
  p.m_output_parameters.outputfreq = 1; //Every generation
  p.m_output_parameters.is_silent = true;
  return p;
}

kewe::parameters kewe::create_parameters_article_figure_3() noexcept
{
  parameters p;
  p.m_sim_parameters = create_sim_parameters_article_figure_3();
  p.m_output_parameters.outputfreq = 1;
  p.m_output_parameters.is_silent = true;
  return p;
}

kewe::parameters kewe::create_profiling_parameters() noexcept
{
  parameters p;
  p.m_sim_parameters.set_end_time(1000); // End simulation at this generation
  p.m_sim_parameters.Nx = 2;         // Number of X alleles
  p.m_sim_parameters.Np = 2;         // Number of P alleles
  p.m_sim_parameters.Nq = 2;         // Number of Q alleles
  p.m_sim_parameters.x0 = 0.5;    // initial x gene
  p.m_sim_parameters.p0 = 0.5;    // initial p gene
  p.m_sim_parameters.q0 = 0.5;    // initial q gene
  p.m_sim_parameters.se = 0.1;    // specificity of mate choice ecological type
  p.m_sim_parameters.sm = 0.1;    // specificity of mate choice mating type
  p.m_sim_parameters.sc = 0.3;    // unction RJCB: Strength of competition
  p.m_sim_parameters.set_eco_res_distribution_width(1.2);
  p.m_sim_parameters.sq = 1.0;    // strength of viability selection on male mating type
  p.m_sim_parameters.set_mut_distr_width(0.02);   // width distribution mutation sizes
  p.m_sim_parameters.c = 0.0005;  // intensity competition
  p.m_sim_parameters.at = 0.05;    // attractivity threshold
  p.m_sim_parameters.seed = 123;
  p.m_sim_parameters.set_ploidy(ploidy::haploid);
  p.m_sim_parameters.popsize = 1000;                // Initial population size
  p.m_output_parameters.outputfreq = 1; //Every generation
  p.m_output_parameters.is_silent = true;
  return p;
}
