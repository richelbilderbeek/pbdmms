#include <iostream>
#include <ctime>

#include <QApplication>

#include "pbd_helper.h"
#include "qtsurfaceplotwidget.h"
#include "histogram_to_png.h"

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
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
  histogram_to_png(csv_filename, png_filename);
}
