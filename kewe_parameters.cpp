#include <cassert>
#include <fstream>
#include <iostream>
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
  /*
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v[0] == "se") { p.m_sim_parameters.m_mate_spec_eco = std::stod(v[1]); }
  }
  */

  const auto lines = file_to_vector(filename);

  parameters p = create_parameters_article_figure_3();

  for (const std::string& line: lines)
  {
    std::vector<std::string> v{seperate_string(line, ' ')};
    if(v[0] == "type0")
    {
      v.erase(v.begin());
      for (int i = 0; i < static_cast<int>(v.size()); ++i)
      {
        switch(i)
        {
          case 0: p.m_sim_parameters.x0 = std::stod(v[0]); break;
          case 1: p.m_sim_parameters.p0 = std::stod(v[1]); break;
          case 2: p.m_sim_parameters.q0 = std::stod(v[2]); break;
          default: throw std::invalid_argument("Too many parameters after \"type0\"");
        }
      }
    }
    else if(v[0] == "seed"){p.m_sim_parameters.seed = std::stod(v[1]);}
    else if(v[0] == "pop0"){p.m_sim_parameters.popsize = std::stod(v[1]);}
    else if(v[0] == "end"){p.m_sim_parameters.set_end_time(std::stod(v[1]));}
    else if(v[0] == "sk")
    {
      p.m_sim_parameters.set_eco_res_distribution_width(std::stod(v[1]));
    }
    else if(v[0] == "sv"){p.m_sim_parameters.set_mut_distr_width(std::stod(v[1]));}
    else if(v[0] == "sq"){p.m_sim_parameters.sq = std::stod(v[1]);}
    else if(v[0] == "at"){p.m_sim_parameters.at = std::stod(v[1]);}
    else if(v[0] == "output")
    {

      v.erase(v.begin());
      assert(v.size() >= 1);
      p.m_output_parameters.outputfreq = std::stod(v[0]);
      if(v.size() >= 2)
      {
        p.m_output_parameters.outputfilename = v[1];
      }
    }
  }

  return parameters(
    read_output_parameters(filename),
    read_simulation_parameters(filename)
  );
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
  output_parameters op;
  op.outputfreq = 1; //Every generation
  op.is_silent = true;
  return parameters(
    op,
    create_test_sim_parameters_haploid_1()
  );
}

kewe::parameters kewe::create_parameters_article_figure_3() noexcept
{
  output_parameters op;
  op.outputfreq = 1;
  op.is_silent = true;
  return parameters(
    op,
    create_sim_parameters_article_figure_3()
  );
}

kewe::parameters kewe::create_profiling_parameters() noexcept
{
  output_parameters op;
  op.outputfreq = 0; //Only in the end
  op.is_silent = true;
  return parameters(
    op,
    create_sim_parameters_profiling()
  );
}

kewe::parameters kewe::create_random_run_parameters() noexcept
{
  output_parameters op;
  op.outputfreq = 0; //Only in the end
  op.is_silent = true;
  return parameters(
    op,
    create_sim_parameters_random()
  );
}

bool kewe::is_valid(const parameters& p) noexcept
{
  return is_valid(p.m_output_parameters)
    && is_valid(p.m_sim_parameters)
  ;
}

std::ostream& kewe::operator<<(std::ostream& os, const parameters p) noexcept
{
  os
    << p.m_sim_parameters << '\n'
    << p.m_output_parameters
  ;
  return os;
}

