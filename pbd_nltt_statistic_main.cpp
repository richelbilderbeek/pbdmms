#include <iostream>
#include <ctime>
#include <stdexcept>

#include <QApplication>

#include "pbd_helper.h"
#include "pbd_nltt.h"
//#include "qtsurfaceplotwidget.h"
#include "histogram_to_png.h"

#include <string>

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  if (argc < 2)
  {
    std::cout << "Please supply an argument, for example:\n"
      << "\n"
      << "  ./pbd_nltt_statistic nltt_1.csv nltt_2.csv \n"
      << "\n"
    ;
    return 1;
  }
  const std::string csv_filename_1 = argv[1];
  const std::string csv_filename_2 = argv[2];
  try
  {
    //pbd::get_nltt_statistic()
    const double nltt_statistic{pbd::get_nltt_statistic(csv_filename_1, csv_filename_2)};
    std::cout << nltt_statistic << '\n';
    return 0;
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  return a.exec(); //ribi: I am sorry if this causes the program to have an infinite loop
}
