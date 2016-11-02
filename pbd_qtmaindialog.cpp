#include "pbd_qtmaindialog.h"
#include "ui_pbd_qtmaindialog.h"

pbd::qtmaindialog::qtmaindialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::pbd_qtmaindialog)
{
  ui->setupUi(this);
}

pbd::qtmaindialog::~qtmaindialog()
{
  delete ui;
}
