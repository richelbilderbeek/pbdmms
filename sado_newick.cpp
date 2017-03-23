#include "sado_newick.h"

#include <cassert>
#include <fstream>

#include <boost/algorithm/string/join.hpp>

#include "sado_helper.h"

bool sado::is_newick(const std::string& s)
{
  const std::string r_script_filename{"is_newick.R"};
  const std::string txt_filename{"is_newick.txt"};
  delete_file_if_present(r_script_filename);
  delete_file_if_present(txt_filename);
  //Write R script
  {
    std::ofstream f(r_script_filename);
    f
      << "library(ape)" << '\n'
      << "p <- tryCatch(" << '\n'
      << "  read.tree(text = \"" << s << "\")," << '\n'
      << "  error = function(e) {}" << '\n'
      << ")" << '\n'
      << "c <- ifelse(class(p) == \"phylo\", \"Y\", \"N\")" << '\n'
      << "my_file <- file(\"" << txt_filename << "\")" << '\n'
      << "writeLines(c, my_file)" << '\n'
      << "close(my_file)" << '\n'
    ;
  }
  //Run R script
  {
    assert(is_regular_file(r_script_filename));
    const std::string cmd{"Rscript " + r_script_filename};
    const int error{std::system(cmd.c_str())};
    if (error) return false;
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

std::string sado::to_newick(const ancestry_graph& g)
{
  return to_newick_impl1(g);
}

std::string sado::to_newick_impl1(const ancestry_graph& g)
{
  const int n_taxa{count_n_extant(g)};

  if (n_taxa == 0) { return "(:0);"; }

  const int n_gens{count_n_generations(g)};

  //For one taxon, the Newick is '(:n_gens-1);'
  if (n_taxa == 1) { return "(:" + std::to_string(n_gens-1) + ");"; }

  const auto vds = collect_root_vds(g);
  std::vector<std::string> newicks;
  std::transform(
    std::begin(vds),
    std::end(vds),
    std::back_inserter(newicks),
    [g](const auto vd)
    {
      return to_newick(vd, g) + ";";
    }
  );

  return boost::algorithm::join(newicks, " ");
}

std::string sado::to_newick(const sp_vert_desc vd, const ancestry_graph& g)
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
      const auto t_younger = g[vd_sub].get_generation();
      const auto dt = t_younger - t;
      return to_newick(vd_sub, g) + ":" + std::to_string(dt);
    }
  );
  return "(" + boost::algorithm::join(newicks, ",") + ")";
}

std::string sado::to_newick(
  const sp_vert_desc vd,
  const ancestry_graph& g,
  const int n_generations)
{
  assert(!is_tip(vd, g, n_generations));

  const std::vector<sp_vert_desc> vds{
    collect_younger_nodes(vd, g, n_generations)
  };
  if (vds.size() == 1)
  {
    const auto t_younger = g[vds[0]].get_generation();
    const auto t = g[vd].get_generation();
    const auto dt = t_younger - t;
    return to_newick(vds[0], g, n_generations) + ":" + std::to_string(dt);
  }

  std::vector<std::string> newicks;
  const auto t = g[vd].get_generation();
  std::transform(
    std::begin(vds),
    std::end(vds),
    std::back_inserter(newicks),
    [g, t, n_generations](const auto vd_sub)
    {
      if (is_tip(vd_sub, g, n_generations))
      {
        const auto t_younger = g[vd_sub].get_generation();
        const auto dt = t_younger - t;
        return ":" + std::to_string(dt);
      }
      const auto t_younger = g[vd_sub].get_generation();
      const auto dt = t_younger - t;
      return to_newick(vd_sub, g, n_generations) + ":" + std::to_string(dt);
    }
  );
  return "(" + boost::algorithm::join(newicks, ",") + ")";
}
