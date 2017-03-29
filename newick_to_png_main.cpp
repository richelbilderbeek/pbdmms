#include <cassert>
#include <iostream>
#include <QApplication>
#include "sado_newick.h"

using namespace sado;


int main(int argc, char *argv[])
{
  QApplication a(argc, argv); //!OCLINT a is used in the background
  if (argc != 2)
  {
    std::cout
      << "Invalid number of arguments\n"
      << '\n'
      << "Use:\n"
      << '\n'
      << "./newick_to_png [newick]\n"
      << '\n'
      << "The Newick will be saved to newick.png\n"
      << "For example:"
      << '\n'
      << "./newick_to_png (:1,:1):1;\n"
    ;
  }
  newick_to_picture(argv[1], "newick.png");

}

