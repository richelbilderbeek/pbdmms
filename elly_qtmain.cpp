#include "elly_qtmaindialog.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QApplication>
#pragma GCC diagnostic pop

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  elly::qtmaindialog w;
  w.show();
  if (argc == 2 && std::string(argv[1]) == "--run")
  {
    w.on_start_clicked();
  }
  return a.exec();
}
