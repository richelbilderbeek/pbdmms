#ifndef RIBI_QTMAINDIALOG_H
#define RIBI_QTMAINDIALOG_H

#include "ribi_parameters.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QDialog>
#pragma GCC diagnostic pop

namespace Ui {
  class ribi_qtmaindialog;
}

namespace ribi{

class qtmaindialog : public QDialog
{
  Q_OBJECT //!OCLINT

public:
  explicit qtmaindialog(QWidget *parent = 0);
  qtmaindialog(const qtmaindialog&) = delete;
  qtmaindialog& operator=(const qtmaindialog&) = delete;
  ~qtmaindialog();

  ///Will throw if current values are invalid
  parameters create_parameters() const;

private slots:
  void on_button_clicked();

private:
  Ui::ribi_qtmaindialog *ui;

};

} //~namespace ribi

#endif // RIBI_QTMAINDIALOG_H
