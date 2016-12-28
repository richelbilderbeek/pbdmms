#include "sado_parameters.h"

#include <fstream>
#include <iostream>
#include <cstring>
#include "sado_globals.h"
#include "sado_helper.h"


sado::parameters::parameters(
    const int pop_size,
    const erasure_method e,
    const bool use_initialization_bug
  )
  : m_erasure{e},
    m_pop_size{pop_size},
    m_use_initialization_bug{use_initialization_bug}
{

}

sado::parameters sado::readparameters(const std::string& filename)
{
  using std::strcmp;
  using std::cout;

  int pop_size{-1};
  std::ifstream fp(filename);
  char s[50],outputfilename[50];
  std::cout<<"reading parameters and initializing\n";
  if(!fp) exit(1);
  std::cout<<"opening parameterfile"<<'\n';
  while(fp>>s)
    {
      if(strcmp(s,"histbin")==0) { fp>>histbinx>>histbinp>>histbinq; cout<<"parameters "<<s<<" set to "<<histbinx<<" "<<histbinp<<" "<<histbinq<<'\n';}
      if(strcmp(s,"seed")==0) {fp>>seed; cout<<"parameter "<<s<<" set to "<<seed<<'\n';}
      if(strcmp(s,"pop0")==0) { fp >> pop_size; cout <<"parameter "<< s << " set to "<< pop_size << '\n'; }
      if(strcmp(s,"type0")==0)
        {
          fp>>x0>>p0>>q0;
          cout<<"parameter x0 set to "<<x0<<'\n';
          cout<<"parameter p0 set to "<<p0<<'\n';
          cout<<"parameter q0 set to "<<q0<<'\n';
        }
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
        out.open(outputfilename);
        if(!out) {cout<<"unable to open datafile"<<'\n'; exit(1);}
      }
    }
  fp.close();

  return parameters(
    read_pop_size(filename),
    read_erasure_method(filename),
    read_use_initialization_bug(filename)
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

int sado::read_pop_size(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v.at(0) == "pop0") { return std::stoi(v.at(1)); }
  }
  throw std::runtime_error("parameter pop0 not found");

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
