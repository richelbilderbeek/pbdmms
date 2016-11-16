#ifndef KEWE_QTDIALOG_H
#define KEWE_QTDIALOG_H

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QDialog>
#include "kewe_parameters.h"
#pragma GCC diagnostic pop

namespace Ui {
class kewe_qtdialog;
}

namespace kewe {

class qtdialog : public QDialog
{
  Q_OBJECT //!OCLINT

public:
  explicit qtdialog(QWidget *parent = 0);
  qtdialog(const qtdialog&) = delete;
  qtdialog& operator=(const qtdialog&) = delete;
  ~qtdialog();

  kewe_parameters get_parameters() const;

private slots:
  void on_start_clicked();

  void on_checkBox_clicked();

  void on_checkBox_2_clicked();

private:
  Ui::kewe_qtdialog *ui;
};

} //~namespace kewe

#endif // KEWE_QTDIALOG_H
