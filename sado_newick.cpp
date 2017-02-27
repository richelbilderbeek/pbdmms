#include "sado_newick.h"

#include <cassert>
#include <fstream>

#include <boost/algorithm/string/join.hpp>

#include "sado_helper.h"

bool sado::is_newick(const std::string& s)
{
  const std::string r_script_filename{"is_newick.R"};
  const std::string txt_filename{"is_newick.txt"};
  //Write R script
  {
    std::ofstream f(r_script_filename);
    f
      << "library(ape)" << '\n'
      << "p <- read.tree(text = \"" << s << "\")" << '\n'
      << "c <- ifelse(is.null(p), \"N\", \"Y\")" << '\n'
      << "my_file <- file(\"" << txt_filename << "\")" << '\n'
      << "writeLines(c, my_file)" << '\n'
      << "close(my_file)" << '\n'
    ;
  }
  //Run R script
  {
    assert(is_regular_file(r_script_filename));
    const std::string cmd{"Rscript " + r_script_filename};
    std::system(cmd.c_str());
  }
  //Parse results
  assert(is_regular_file(txt_filename));
  const auto lines = file_to_vector(txt_filename);
  assert(!lines.empty());
  assert(!lines.front().empty());
  const char success{lines[0][0]};
  assert(success == 'Y' || success == 'N');
  return success == 'Y';
}


std::string sado::newick_surround(const std::string& s)
{
  return "(" + s + ");";
}

std::string sado::to_newick(const species_graph& g)
{
  const int n_taxa{count_n_extant(g)};

  if (n_taxa == 0) { return "(:0);"; }

  const int n_gens{count_n_generations(g)};

  //For one taxon, the Newick is '(:n_gens-1);'
  if (n_taxa == 1) { return "(:" + std::to_string(n_gens-1) + ");"; }

  //
  const auto vds = collect_root_vds(g);
  std::vector<std::string> newicks;
  std::transform(
    std::begin(vds),
    std::end(vds),
    std::back_inserter(newicks),
    [g](const auto vd)
    {
      return "(" + to_newick(vd, g) + ");";
    }
  );
  return boost::algorithm::join(newicks, " ");
}

std::string sado::to_newick(const sp_vert_desc vd, const species_graph& g)
{
  assert(!is_tip(vd, g));
  const std::vector<sp_vert_desc> vds{
    collect_younger_nodes(vd, g)
  };
  if (vds.size() == 1)
  {
    const auto t_younger = g[vds[0]].get_generation();
    const auto t = g[vd].get_generation();
    const auto dt = t_younger - t;
    return to_newick(vds[0], g) + ":" + std::to_string(dt);
  }
  std::vector<std::string> newicks;
 const auto t = g[vd].get_generation();
  std::transform(
    std::begin(vds),
    std::end(vds),
    std::back_inserter(newicks),
    [g, t](const auto vd_sub)
    {
      if (is_tip(vd_sub, g))
      {
        const auto t_younger = g[vd_sub].get_generation();
        const auto dt = t_younger - t;
        return ":" + std::to_string(dt);
      }
      else
      {
        return to_newick(vd_sub, g);
      }
    }
  );
  return "(" + boost::algorithm::join(newicks, ",") + ")";
}
