#include "sado_parameters.h"

#include <fstream>
#include <iostream>
#include <cstring>
#include <stdexcept>
#include "sado_globals.h"
#include "sado_helper.h"


sado::parameters::parameters(
  const erasure_method e,
  const std::string& output_filename,
  const double p0,
  const int pop_size,
  const double q0,
  const int seed,
  const bool use_initialization_bug,
  const double x0
)
  : m_erasure{e},
    m_output_filename{output_filename},
    m_p0{p0},
    m_pop_size{pop_size},
    m_q0{q0},
    m_seed{seed},
    m_use_initialization_bug{use_initialization_bug},
    m_x0{x0}
{

}

void sado::create_testrun_file(const std::string& filename)
{
  std::ofstream f(filename);
  f
    << "alleles 2 2 2\n"
    << "histbin 0.1 0.1 0.1 0.1\n"
    << "seed 123\n"
    << "pop0 100\n"
    << "type0 0.5 0.5 0.5\n"
    << "end 4000\n"
    << "sc 0.3\n"
    << "se 0.1\n"
    << "sk 1.2\n"
    << "c 0.01\n"
    << "sm 0.1\n"
    << "sv 0.02\n"
    << "sq 1.0\n"
    << "eta 1.0\n"
    << "b 4.0\n"
    << "output 10 output.txt\n"
    << "diploid 1\n"
  ;
}


void sado::create_article_file(const std::string& filename)
{
  std::ofstream f(filename);
  f
    << "alleles 1 1 1\n"
    << "histbin 0.1 0.1 0.1 0.1\n"
    << "seed 123\n"
    << "pop0 1000\n"
    << "type0 0.5 0.5 0.5\n"
    << "end 4000\n"
    << "sc 0.4\n"
    << "se 0.6\n"
    << "sk 1.2\n"
    << "c 0.0005\n"
    << "sm 0.2\n"
    << "sv 0.02\n"
    << "sq 1.0\n"
    << "eta 1.0\n"
    << "b 4.0\n"
    << "output 10 output.txt\n"
    << "haploid 1\n"
  ;
}

sado::parameters sado::create_golden_standard()
{
  const std::string temp_filename{"create_golden_standard.txt"};
  create_golden_standard_file(temp_filename);
  return readparameters(temp_filename);
}

void sado::create_golden_standard_file(const std::string& filename)
{
  std::ofstream f(filename);
  f
    << "alleles 1 1 1\n"
    << "histbin 0.1 0.1 0.1 0.1\n"
    << "seed 123\n"
    << "pop0 1000\n"
    << "type0 0.5 0.5 0.5\n"
    << "end 100\n"
    << "sc 0.4\n"
    << "se 0.6\n"
    << "sk 1.2\n"
    << "c 0.0005\n"
    << "sm 0.2\n"
    << "sv 0.02\n"
    << "sq 1.0\n"
    << "eta 1.0\n"
    << "b 4.0\n"
    << "output 10 output.txt\n"
    << "haploid 1\n"
  ;
}

void sado::create_profiling_file(const std::string& filename)
{
  std::ofstream f(filename);
  f
    << "alleles 1 1 1\n"
    << "histbin 0.1 0.1 0.1 0.1\n"
    << "seed 123\n"
    << "pop0 1000\n"
    << "type0 0.5 0.5 0.5\n"
    << "end 100\n"
    << "sc 0.4\n"
    << "se 0.6\n"
    << "sk 1.2\n"
    << "c 0.0005\n"
    << "sm 0.2\n"
    << "sv 0.02\n"
    << "sq 1.0\n"
    << "eta 1.0\n"
    << "b 4.0\n"
    << "output 10 output.txt\n"
    << "haploid 1\n"
    << "erasure_method swap\n"
    << "initialization_bug 0\n"
  ;
}

bool sado::is_golden_standard(const parameters& p) noexcept
{
  //return p == create_golden_standard();
  return
       p.get_use_initialization_bug()
    && p.get_erasure() == erasure_method::erase
  ;
}

sado::parameters sado::readparameters(const std::string& filename)
{
  if (!is_regular_file(filename))
  {
    throw std::invalid_argument("paramater file not found");
  }
  using std::strcmp;
  using std::cout;
  std::ifstream fp(filename);
  char s[50],outputfilename[50];
  std::cout<<"reading parameters and initializing\n";
  std::cout<<"opening parameterfile"<<'\n';
  while(fp>>s)
    {
      if(strcmp(s,"histbin")==0) { fp>>histbinx>>histbinp>>histbinq; cout<<"parameters "<<s<<" set to "<<histbinx<<" "<<histbinp<<" "<<histbinq<<'\n';}
      //if(strcmp(s,"seed")==0) {fp>>seed; cout<<"parameter "<<s<<" set to "<<seed<<'\n';}
      //if(strcmp(s,"type0")==0)
      //  {
      //    fp>>x0>>p0>>q0;
      //    cout<<"parameter x0 set to "<<x0<<'\n';
      //    cout<<"parameter p0 set to "<<p0<<'\n';
      //    cout<<"parameter q0 set to "<<q0<<'\n';
      //  }
      if(strcmp(s,"end")==0) {fp>>endtime;cout<<"parameter "<<s<<" set to "<<endtime<<'\n';}
      if(strcmp(s,"sc")==0) {fp>>sc;cout<<"parameter "<<s<<" set to "<<sc<<'\n';}
      if(strcmp(s,"se")==0) {fp>>se;cout<<"parameter "<<s<<" set to "<<se<<'\n';}
      if(strcmp(s,"sm")==0) {fp>>sm;cout<<"parameter "<<s<<" set to "<<sm<<'\n';}
      if(strcmp(s,"sv")==0) {fp>>sv;cout<<"parameter "<<s<<" set to "<<sv<<'\n';}
      if(strcmp(s,"sq")==0) {fp>>sq;cout<<"parameter "<<s<<" set to "<<sq<<'\n';}
      if(strcmp(s,"sk")==0) {fp>>sk;cout<<"parameter "<<s<<" set to "<<sk<<'\n';}
      if(strcmp(s,"c")==0) {fp>>c;cout<<"parameter "<<s<<" set to "<<c<<'\n';}
      if(strcmp(s,"b")==0) {fp>>b;cout<<"parameter "<<s<<" set to "<<b<<'\n';}
      if(strcmp(s,"eta")==0) {fp>>eta;cout<<"parameter "<<s<<" set to "<<eta<<'\n';}
      if(strcmp(s,"output")==0)
      {
        fp>>outputfreq>>outputfilename;
        cout<<"saving data every "<<outputfreq<<" generations in "<<outputfilename<<'\n';
      }
    }
  fp.close();

  return parameters(
    read_erasure_method(filename),
    read_output_filename(filename),
    read_p0(filename),
    read_pop_size(filename),
    read_q0(filename),
    read_seed(filename),
    read_use_initialization_bug(filename),
    read_x0(filename)
  );
}


sado::erasure_method sado::read_erasure_method(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v[0] == "erasure_method") { return to_erasure_method(v[1]); }
  }
  return erasure_method::erase;
}

std::string sado::read_output_filename(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v.at(0) == "output") { return v.at(2); }
  }
  throw std::runtime_error("parameter 'output'' not found");

}

double sado::read_p0(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v.at(0) == "type0") { return std::stod(v.at(2)); }
  }
  throw std::runtime_error("parameter 'p0' not found");
}

int sado::read_pop_size(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v.at(0) == "pop0") { return std::stoi(v.at(1)); }
  }
  throw std::runtime_error("parameter 'pop0' not found");
}

double sado::read_q0(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v.at(0) == "type0") { return std::stod(v.at(3)); }
  }
  throw std::runtime_error("parameter 'q0' not found");
}

int sado::read_seed(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v.at(0) == "seed") { return std::stoi(v.at(1)); }
  }
  throw std::runtime_error("parameter 'seed' not found");

}

bool sado::read_use_initialization_bug(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v.at(0) == "use_initialization_bug") { return std::stoi(v.at(1)); }
  }
  return true;
}

double sado::read_x0(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v.at(0) == "type0") { return std::stod(v.at(1)); }
  }
  throw std::runtime_error("parameter 'x0' not found");
}

bool sado::operator==(const parameters& lhs, const parameters& rhs) noexcept
{
  return lhs.get_erasure() == rhs.get_erasure()
    && lhs.get_output_filename() == rhs.get_output_filename()
    && lhs.get_pop_size() == rhs.get_pop_size()
    && lhs.get_use_initialization_bug() == rhs.get_use_initialization_bug()
  ;
}
