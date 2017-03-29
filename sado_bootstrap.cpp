#include "sado_bootstrap.h"

#include <cassert>
#include <fstream>
#include <iostream>

#include "sado_helper.h"
#include "sado_newick.h"

sado::bootstrap::bootstrap(
  const std::string& newick,
  const likelihood& max_likelihood,
  const int n,
  const int rng_seed)
  : m_likelihoods{do_bootstrap(newick, max_likelihood, n, rng_seed)}
{

}

std::vector<sado::likelihood> sado::do_bootstrap(
  const std::string& newick,
  const likelihood& max_likelihood,
  const int n,
  const int rng_seed
)
{
  assert(n >= 0);
  assert(!newick.empty());
  assert(is_newick(newick));
  const std::string r_script_filename{"do_bootstrap.R"};
  const std::string csv_filename{"do_bootstrap.csv"};
  delete_file_if_present(r_script_filename);
  delete_file_if_present(csv_filename);
  //Write R script
  {
    std::ofstream f(r_script_filename);
    f
      << "library(ape)" << '\n'
      << "library(PBD)" << '\n'
      << "newick <- \"" << newick << "\"" << '\n'
      << "phylogeny <- read.tree(text = newick)" << '\n'
      << "brts <- branching.times(phylogeny)" << '\n'
      << "init_b <- " << max_likelihood.get_b() << '\n'
      << "init_mu_1 <- " << max_likelihood.get_mu_1()  << "  # extinction rate of good species" << '\n'
      << "init_la_1 <- " << max_likelihood.get_lambda_1() << " # speciation-completion rate" << '\n'
      << "" << '\n'
      << "# The initial values of the parameters that must be optimized" << '\n'
      << "initparsopt <- c(init_b, init_mu_1, init_la_1)" << '\n'
      << "" << '\n'
      << "# The extinction rates between incipient and good species are equal" << '\n'
      << "exteq <- TRUE" << '\n'
      << "" << '\n'
      << "# The first element of the branching times is the crown age (and not the stem age)" << '\n'
      << "soc <- 2" << '\n'
      << "" << '\n'
      << "# Conditioning on non-extinction of the phylogeny" << '\n'
      << "# as I actively selected for a nice phylogeny" << '\n'
      << "cond <- 1" << '\n'
      << "" << '\n'
      << "# Give the likelihood of the phylogeny (instead of the likelihood of the branching times)" << '\n'
      << "btorph <- 1" << '\n'
      << "" << '\n'
      << "# The number of simulations for the bootstrap" << '\n'
      << "endmc <- " << n << '\n'
      << "" << '\n'
      << "b <- pbd_bootstrap(" << '\n'
      << "  brts = brts," << '\n'
      << "  initparsopt = initparsopt," << '\n'
      << "  exteq = exteq," << '\n'
      << "  soc = soc," << '\n'
      << "  cond = cond," << '\n'
      << "  btorph = btorph," << '\n'
      << "  plotltt = FALSE," << '\n'
      << "  endmc = endmc," << '\n'
      << "  seed = " << rng_seed << '\n'
      << ")" << '\n'
      << "write.csv(b[[3]], \"" << csv_filename << "\")" << '\n'
    ;
  }
  //Run R script
  {
    assert(is_regular_file(r_script_filename));
    const std::string cmd{"Rscript " + r_script_filename};
    const int error{std::system(cmd.c_str())};
    if (error)
    {
      throw std::runtime_error("script do_bootstrap.R failed");
    }
  }
  //Parse results
  return likelihoods_from_file(csv_filename);
}

std::vector<sado::likelihood> sado::likelihoods_from_file(const std::string& filename)
{
  assert(is_regular_file(filename));
  const auto lines = file_to_vector(filename);
  assert(lines.size() >= 2);

  std::vector<likelihood> likelihoods;
  likelihoods.reserve(lines.size() - 1);
  std::transform(
    std::begin(lines) + 1,
    std::end(lines),
    std::back_inserter(likelihoods),
    [](const std::string& line)
    {
      const auto fields = seperate_string(line, ',');
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
  );
  return likelihoods;
}

/*

          b      mu_1  lambda_1      mu_2    loglik df conv
1 0.7547398 0.8414671 0.7793264 0.8414671 -2.393492  3    0

*/
std::ostream& sado::operator<<(std::ostream& os, const bootstrap& b) noexcept
{
  os << create_header_likelihood() << '\n';
  int i{1};

  for (const auto& likelihood: b.m_likelihoods)
  {
    os
      << i << '\t'
      << likelihood.get_b() << '\t'
      << likelihood.get_mu_1() << '\t'
      << likelihood.get_lambda_1() << '\t'
      << likelihood.get_mu_2() << '\t'
      << likelihood.get_loglik() << '\t'
      << likelihood.get_df() << '\t'
      << likelihood.get_conv() << '\n';
    ;
    ++i;
  }
  return os;
}


