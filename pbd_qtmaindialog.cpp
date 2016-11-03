#include "pbd_qtmaindialog.h"

#include "pbd.h"
#include "ui_pbd_qtmaindialog.h"

pbd::qtmaindialog::qtmaindialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::pbd_qtmaindialog)
{
  ui->setupUi(this);
  on_start_clicked();
}

pbd::qtmaindialog::~qtmaindialog()
{
  delete ui;
}


void pbd::qtmaindialog::on_start_clicked()
{
  const double birth_good{
    ui->parameters->item(0,0)->text().toDouble()
  };
  const double birth_incipient{
    ui->parameters->item(1,0)->text().toDouble()
  };
  const double completion{
    ui->parameters->item(2,0)->text().toDouble()
  };
  const double death_good{
    ui->parameters->item(3,0)->text().toDouble()
  };
  const double death_incipient{
    ui->parameters->item(4,0)->text().toDouble()
  };
  const double time{
    ui->parameters->item(5,0)->text().toDouble()
  };
  const int seed{
    ui->parameters->item(6,0)->text().toInt()
  };
  const std::string filename{"pbd_qtmaindialog_on_start_clicked.png"};
  pbd::run_pbd_sim(
    birth_good,
    birth_incipient,
    completion,
    death_good,
    death_incipient,
    time,
    seed,
    filename
  );
  QPixmap p;
  p.load(filename.c_str());
  ui->result->setPixmap(p);
}
