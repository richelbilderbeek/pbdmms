#include <iostream>
#include <ctime>
#include <fstream>
#include <stdexcept>

#include <QApplication>

#include "pbd_helper.h"
#include "pbd_nltt.h"
//#include "qtsurfaceplotwidget.h"
#include "histogram_to_png.h"

#include <string>

void show_help() noexcept
{
  std::cout
    << "pbd_nltt_average" << '\n'
    << "------------------" << '\n'
    << "" << '\n'
    << "Program to infer the average nLTT of multiple saved nLTT plots" << '\n'
    << "" << '\n'
    << "Use: " << '\n'
    << "" << '\n'
    << "  Create two example nLTT files."
    << "  These will have filenames 'nltt_1.csv' and 'nltt_2.csv'." << '\n'
    << "  Will do nothing if these files already exist." << '\n'
    << "" << '\n'
    << "    ./pbd_nltt_average --create" << '\n'
    << "" << '\n'
    << "  Conclude the average nLTT of two files with "<< '\n'
    << "    filenames 'nltt_1.csv' and 'nltt_2.csv', write" << '\n'
    << "    results to results.csv" << '\n'
    << "" << '\n'
    << "    ./pbd_nltt_average nltt_1.csv nltt_2.csv results.csv" << '\n'
    << "" << '\n';
}

void create_example_nltt_1() noexcept
{
  const std::string filename{"nltt_1.csv"};
  if (!pbd::is_regular_file(filename))
  {
    std::ofstream f(filename);
    f << pbd::create_test_nltt_1();
  }
}

void create_example_nltt_2() noexcept
{
  const std::string filename{"nltt_2.csv"};
  if (!pbd::is_regular_file(filename))
  {
    std::ofstream f(filename);
    f << pbd::create_test_nltt_2();
  }
}

void create_example_nltts() noexcept
{
  create_example_nltt_1();
  create_example_nltt_2();
}


int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  if (argc < 2)
  {
    show_help();
    return 0;
  }
  if (std::string(argv[1]) == "--create")
  {
    create_example_nltts();
    return 0;
  }

  try
  {
    auto args = pbd::get_args(argc, argv);
    const std::string output_filename = args.back();
    args.pop_back();
    const pbd::nltt n = pbd::calc_average(pbd::load_nltts_from_csvs(args));
    std::ofstream f(output_filename);
    f << n;
    return 0;
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << '\n';
    return 1;
  }
  return a.exec(); //ribi: I am sorry if this causes the program to have an infinite loop
}
