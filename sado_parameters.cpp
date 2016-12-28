#include "sado_parameters.h"

#include <fstream>
#include <iostream>
#include <cstring>
#include <stdexcept>
#include "sado_helper.h"

sado::parameters::parameters(
  const double b,
  const double c,
  const int end_time,
  const erasure_method e,
  const double eta,
  const double histbinp,
  const double histbinq,
  const double histbinx,
  const next_generation_method next_gen_method,
  const std::string& output_filename,
  const int output_freq,
  const double p0,
  const int pop_size,
  const double q0,
  const double sc,
  const double se,
  const int seed,
  const double sk,
  const double sm,
  const double sq,
  const double sv,
  const bool use_initialization_bug,
  const double x0
)
  :
    m_b{b},
    m_c{c},
    m_end_time{end_time},
    m_erasure{e},
    m_eta{eta},
    m_histbinp{histbinp},
    m_histbinq{histbinq},
    m_histbinx{histbinx},
    m_next_gen_method{next_gen_method},
    m_output_filename{output_filename},
    m_output_freq{output_freq},
    m_p0{p0},
    m_pop_size{pop_size},
    m_q0{q0},
    m_sc{sc},
    m_se{se},
    m_seed{seed},
    m_sk{sk},
    m_sm{sm},
    m_sq{sq},
    m_sv{sv},
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

sado::parameters sado::create_article_parameters()
{
  const std::string temp_filename{"create_article_parameters.txt"};
  create_article_file(temp_filename);
  return readparameters(temp_filename);
}


sado::parameters sado::create_golden_standard_parameters()
{
  const std::string temp_filename{"create_golden_standard_parameters.txt"};
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

sado::parameters sado::create_profiling_parameters()
{
  const std::string temp_filename{"create_profiling_parameters.txt"};
  create_profiling_file(temp_filename);
  return readparameters(temp_filename);
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
  return parameters(
    read_b(filename),
    read_c(filename),
    read_end_time(filename),
    read_erasure_method(filename),
    read_eta(filename),
    read_histbinp(filename),
    read_histbinq(filename),
    read_histbinx(filename),
    read_next_gen_method(filename),
    read_output_filename(filename),
    read_output_freq(filename),
    read_p0(filename),
    read_pop_size(filename),
    read_q0(filename),
    read_sc(filename),
    read_se(filename),
    read_seed(filename),
    read_sk(filename),
    read_sm(filename),
    read_sq(filename),
    read_sv(filename),
    read_use_initialization_bug(filename),
    read_x0(filename)
  );
}

double sado::read_b(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v.at(0) == "b") { return std::stod(v.at(1)); }
  }
  throw std::runtime_error("parameter 'b' not found");
}

double sado::read_c(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v.at(0) == "c") { return std::stod(v.at(1)); }
  }
  throw std::runtime_error("parameter 'c' not found");
}

int sado::read_end_time(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v.at(0) == "end") { return std::stoi(v.at(1)); }
  }
  throw std::runtime_error("parameter 'end' not found");

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

double sado::read_eta(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v.at(0) == "eta") { return std::stod(v.at(1)); }
  }
  throw std::runtime_error("parameter 'eta' not found");
}

double sado::read_histbinp(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v.at(0) == "histbin") { return std::stod(v.at(2)); }
  }
  throw std::runtime_error("parameter 'histbinp' not found");
}

double sado::read_histbinq(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v.at(0) == "histbin") { return std::stod(v.at(3)); }
  }
  throw std::runtime_error("parameter 'histbinq' not found");
}

double sado::read_histbinx(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v.at(0) == "histbin") { return std::stod(v.at(1)); }
  }
  throw std::runtime_error("parameter 'histbinx' not found");
}


sado::next_generation_method sado::read_next_gen_method(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v[0] == "next_gen_method") { return to_next_gen_method(v[1]); }
  }
  return next_generation_method::overlapping;
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

int sado::read_output_freq(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v.at(0) == "output") { return std::stoi(v.at(1)); }
  }
  throw std::runtime_error("parameter 'outputfreq' not found");
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

double sado::read_sc(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v.at(0) == "sc") { return std::stod(v.at(1)); }
  }
  throw std::runtime_error("parameter 'sc' not found");
}

double sado::read_se(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v.at(0) == "se") { return std::stod(v.at(1)); }
  }
  throw std::runtime_error("parameter 'se' not found");
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

double sado::read_sk(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v.at(0) == "sk") { return std::stod(v.at(1)); }
  }
  throw std::runtime_error("parameter 'sk' not found");
}

double sado::read_sm(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v.at(0) == "sm") { return std::stod(v.at(1)); }
  }
  throw std::runtime_error("parameter 'sm' not found");
}

double sado::read_sq(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v.at(0) == "sq") { return std::stod(v.at(1)); }
  }
  throw std::runtime_error("parameter 'sq' not found");
}

double sado::read_sv(const std::string& filename)
{
  const auto lines = file_to_vector(filename);
  for (const std::string& line: lines)
  {
    const std::vector<std::string> v{seperate_string(line, ' ')};
    if(v.at(0) == "sv") { return std::stod(v.at(1)); }
  }
  throw std::runtime_error("parameter 'sv' not found");
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
