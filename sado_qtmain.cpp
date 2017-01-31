#include "sado_qtdialog.h"
#include <cassert>
#include <locale>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QApplication>
#pragma GCC diagnostic pop

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  std::setlocale(LC_ALL, "en_US.UTF-8");
  assert(std::stod("0.005") > 0.004);
  sado::qtdialog w;
  w.showFullScreen();
  if (argc == 2 && std::string(argv[1]) == "--run")
  {
    w.on_start_clicked();
  }
  return a.exec();
}
