#include "sado_newick.h"

#include <cassert>
#include <fstream>

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


std::string sado::to_newick(const species_graph& /* g */)
{
  //std::string n{"(A,B)"};
  return "(:2);"; //STUB
}
