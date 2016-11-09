#ifndef PBD_QTMAINDIALOG_H
#define PBD_QTMAINDIALOG_H

#include <QDialog>
#include "pbd_l_table.h"

namespace Ui {
class pbd_qtmaindialog;
}

namespace pbd {

class qtmaindialog : public QDialog
{
  Q_OBJECT

public:
  explicit qtmaindialog(QWidget *parent = 0);
  ~qtmaindialog();

private slots:

  void on_start_clicked();

private:
  Ui::pbd_qtmaindialog *ui;

  void display_l_table(const l_table& t);
  void display_png(const std::string& png_filename);
  void resize_table(const int n_rows);
};

} //~namespace pbd

#endif // PBD_QTMAINDIALOG_H
