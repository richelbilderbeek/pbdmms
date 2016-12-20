#include <iostream>
#include <ctime>
#include <stdexcept>

#include <QApplication>

#include "pbd_helper.h"
#include "qtsurfaceplotwidget.h"
#include "histogram_to_png.h"

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  run_tests();
  if (argc < 2)
  {
    std::cout << "Please supply an argument, for example:\n"
      << "\n"
      << "  ./histogram_to_png histogram.csv\n"
      << "  ./histogram_to_png histogram.csv histogram.png\n"
      << "\n"
    ;
    return 1;
  }
  const std::string csv_filename = argv[1];
  const std::string png_filename = argc == 2 ? argv[1] : argv[2];
  try
  {
    histogram_to_png(csv_filename, png_filename);
    if (!pbd::is_regular_file(png_filename))
    {
      std::cerr << "Failed creating " << png_filename << '\n';
      return 1;
    }
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  return a.exec(); //ribi: I am sorry if this causes the program to have an infinite loop
}
