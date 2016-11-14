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
  return std::stof(s);
}

kewe_parameters read_parameters(const std::string& filename)
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
        parameters.output_parameters.outputfreq = str_to_double(v[1]);
        parameters.output_parameters.outputfilename = v[2];
        ///TODO: Change to switchstatement
      }

    ///TODO: Add Diploid/haploid option
    }


  return parameters;
}

void create_test_parameter_file(const std::string& filename)
{
  std::ofstream f(filename.c_str());
  f << "STUB";
}
