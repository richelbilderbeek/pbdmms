#include "sado_likelihood.h"

#include <cassert>
#include <fstream>

#include "sado_helper.h"
#include "sado_newick.h"

sado::likelihood::likelihood(
  const double b,
  const int conv,
  const int df,
  const double lambda_1,
  const double loglik,
  const double mu_1,
  const double mu_2
) : m_b{b},
    m_conv{conv},
    m_df{df},
    m_lambda_1{lambda_1},
    m_loglik{loglik},
    m_mu_1{mu_1},
    m_mu_2{mu_2}
{
  assert(m_b >= 0.0);
  assert(m_conv >= 0);
  assert(m_df >= 0);
  assert(m_lambda_1 >= 0.0);
  assert(m_mu_1 >= 0.0);
  assert(m_mu_2 >= 0.0);
}



/*
          b      mu_1  lambda_1      mu_2    loglik df conv
1 0.7547398 0.8414671 0.7793264 0.8414671 -2.393492  3    0

*/
sado::likelihood sado::likelihood_from_file(const std::string& filename)
{
  assert(is_regular_file(filename));
  const auto lines = file_to_vector(filename);
  assert(lines.size() >= 2);
  const auto fields = seperate_string(lines[1], ',');
  assert(fields.size() == 8);
  const double b{std::stod(fields[1])};
  const double mu_1{std::stod(fields[2])};
  const double lambda_1{std::stod(fields[3])};
  const double mu_2{std::stod(fields[4])};
  const double loglik{std::stod(fields[5])};
  const int df{std::stoi(fields[6])};
  const int conv{std::stoi(fields[7])};
  return likelihood(
    b,
    conv,
    df,
    lambda_1,
    loglik,
    mu_1,
    mu_2
  );
}

sado::likelihood sado::calc_max_likelihood(const std::string& newick)
{
  assert(is_newick(newick));
  const std::string r_script_filename{"calc_max_likelihood.R"};
  const std::string txt_filename{"calc_max_likelihood.txt"};
  delete_file_if_present(r_script_filename);
  delete_file_if_present(txt_filename);
  //Write R script
  {
    std::ofstream f(r_script_filename);
    f
      << "library(ape)" << '\n'
      << "library(PBD)" << '\n'
      << "sink(file = tempfile())" << '\n' //Remove spurious output
      << "ml <- PBD::pbd_ML(" << '\n'
      << "  brts = ape::branching.times(" << '\n'
      << "    read.tree(text = \"" << newick << "\"))," << '\n'
      << "  initparsopt = c(0.2,0.01,0.3)," << '\n'
      << "  exteq = 1 # mu_1 == mu_2" << '\n'
      << ")" << '\n'
      << "sink()" << '\n'
      << "write.csv(ml, \"" << txt_filename << "\")" << '\n'
    ;
  }
  //Run R script
  {
    assert(is_regular_file(r_script_filename));
    const std::string cmd{"Rscript " + r_script_filename};
    const int error{std::system(cmd.c_str())};
    if (error)
    {
      throw std::runtime_error("script calc_max_likelihood.R failed");
    }
  }
  //Parse results
  return likelihood_from_file(txt_filename);
}

std::string sado::to_str(const likelihood& l) noexcept
{
  std::stringstream s;
  s << l;
  return s.str();
}

std::ostream& sado::operator<<(std::ostream& os, const likelihood& l) noexcept
{

/*
          b      mu_1  lambda_1      mu_2    loglik df conv
1 0.7547398 0.8414671 0.7793264 0.8414671 -2.393492  3    0

*/
  os
    << "b: " << l.get_b() << '\n'
    << "mu_1: " << l.get_mu_1() << '\n'
    << "lambda_1: " << l.get_lambda_1() << '\n'
    << "mu_2: " << l.get_mu_2() << '\n'
    << "loglik: " << l.get_loglik() << '\n'
    << "df: " << l.get_df() << '\n'
    << "conv: " << l.get_conv()
  ;
  return os;

}
