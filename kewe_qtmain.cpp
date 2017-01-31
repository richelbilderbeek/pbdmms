#include "kewe_qtdialog.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QApplication>
#pragma GCC diagnostic pop

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  kewe::qtdialog w;
  w.show();
  return a.exec();
}
