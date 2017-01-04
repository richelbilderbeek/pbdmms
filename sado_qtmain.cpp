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
  return a.exec();
}
