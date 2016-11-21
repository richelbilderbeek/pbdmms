#include <cassert>
#include <fstream>
#include <vector>
#include <algorithm>
#include <boost/algorithm/string/split.hpp>
#include "kewe_parameters.h"
#include "kewe_SES.h"

///Determines if a filename is a regular file
///From http://www.richelbilderbeek.nl/CppIsRegularFile.htm
bool is_regular_file(const std::string& filename) noexcept
{
  std::fstream f;
  f.open(filename.c_str(),std::ios::in);
  return f.is_open();
}

///FileToVector reads a file and converts it to a std::vector<std::string>
///From http://www.richelbilderbeek.nl/CppFileToVector.htm
std::vector<std::string> file_to_vector(const std::string& filename)
{
  assert(is_regular_file(filename));
  std::vector<std::string> v;
  std::ifstream in(filename.c_str());
  for (int i=0; !in.eof(); ++i)
  {
    std::string s;
    std::getline(in,s);
    v.push_back(s);
  }
  return v;
}

///From http://www.richelbilderbeek.nl/CppSeperateString.htm
std::vector<std::string> seperate_string(
  const std::string& input,
  const char seperator)
{
  std::vector<std::string> v;
  boost::algorithm::split(v,input,
  std::bind2nd(std::equal_to<char>(),seperator),
  boost::algorithm::token_compress_on);
  return v;
}


//From http://www.richelbilderbeek.nl/CppStrToDouble.htm
double str_to_double(const std::string& s)
{
  return std::stod(s);
}

kewe_parameters read_parameters(const std::string& filename) //!OCLINT Readable, no easier way to read parameters.
{
  const auto lines = file_to_vector(filename);

  kewe_parameters parameters;

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
            case 0: parameters.sim_parameters.Nx = str_to_double(v[0]); break;
            case 1: parameters.sim_parameters.Np = str_to_double(v[1]); break;
            case 2: parameters.sim_parameters.Nq = str_to_double(v[2]); break;
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
            case 0: parameters.sim_parameters.x0 = str_to_double(v[0]); break;
            case 1: parameters.sim_parameters.p0 = str_to_double(v[1]); break;
            case 2: parameters.sim_parameters.q0 = str_to_double(v[2]); break;
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
            case 0: parameters.output_parameters.histbinx = str_to_double(v[0]); break;
            case 1: parameters.output_parameters.histbinp = str_to_double(v[1]); break;
            case 2: parameters.output_parameters.histbinq = str_to_double(v[2]); break;
            default: throw std::invalid_argument("Too many parameters after \"histbin\"");
          }
        }
      }
    else if(v[0] == "seed"){parameters.sim_parameters.seed = str_to_double(v[1]);}
    else if(v[0] == "pop0"){parameters.sim_parameters.popsize = str_to_double(v[1]);}
    else if(v[0] == "end"){parameters.sim_parameters.endtime = str_to_double(v[1]);}
    else if(v[0] == "sc"){parameters.sim_parameters.sc = str_to_double(v[1]);}
    else if(v[0] == "se"){parameters.sim_parameters.se = str_to_double(v[1]);}
    else if(v[0] == "sk"){parameters.sim_parameters.sk = str_to_double(v[1]);}
    else if(v[0] == "c"){parameters.sim_parameters.c = str_to_double(v[1]);}
    else if(v[0] == "sm"){parameters.sim_parameters.sm = str_to_double(v[1]);}
    else if(v[0] == "sv"){parameters.sim_parameters.sv = str_to_double(v[1]);}
    else if(v[0] == "sq"){parameters.sim_parameters.sq = str_to_double(v[1]);}
    else if(v[0] == "eta"){parameters.sim_parameters.eta = str_to_double(v[1]);}
    else if(v[0] == "b"){parameters.sim_parameters.b = str_to_double(v[1]);}
    else if(v[0] == "output")
      {

        v.erase(v.begin());
        assert(v.size() >= 1);
        parameters.output_parameters.outputfreq = str_to_double(v[0]);
        if(v.size() >= 2)
          parameters.output_parameters.outputfilename = v[1];
            }
    else if(v[0] == "haploid")
      {
        if(str_to_double(v[1]) == 1)
          {
            parameters.sim_parameters.haploid = 1;
            parameters.sim_parameters.diploid = 0;
          }
        else if(str_to_double(v[1]) == 0)
          {
            parameters.sim_parameters.haploid = 0;
            parameters.sim_parameters.diploid = 1;
          }
      }
    else if(v[0] == "diploid")
      {
        if(str_to_double(v[1]) == 1)
          {
            parameters.sim_parameters.haploid = 0;
            parameters.sim_parameters.diploid = 1;
          }
        else if(str_to_double(v[1]) == 0)
          {
            parameters.sim_parameters.haploid = 1;
            parameters.sim_parameters.diploid = 0;
          }
      }
    }


  return parameters;
}

void create_test_parameter_file1(const std::string& filename)
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
    << "haploid 1\n";
}

void create_test_parameter_file2(const std::string& filename)
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
    << "haploid 1\n";
}
void create_test_parameter_file3(const std::string& filename)
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
    << "haploid 1\n";
}
