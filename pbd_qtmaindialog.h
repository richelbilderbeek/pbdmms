#ifndef PBD_QTMAINDIALOG_H
#define PBD_QTMAINDIALOG_H

#include <QDialog>

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

private:
  Ui::pbd_qtmaindialog *ui;
};

} //~namespace pbd

#endif // PBD_QTMAINDIALOG_H
