#include "kewe_qtdialog.h"

#include "kewe_simulation.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "ui_kewe_qtdialog.h"
#pragma GCC diagnostic pop

kewe::qtdialog::qtdialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::kewe_qtdialog)
{
  ui->setupUi(this);
  on_start_clicked();
}

kewe::qtdialog::~qtdialog()
{
  delete ui;
}

void kewe::qtdialog::on_start_clicked()
{
  simulation s;
  s.run();
  const auto r = s.get_results();
  ui->eco_trait->SetSurfaceGrey(r.m_ecological_trait);
  ui->male_sexual_trait->SetSurfaceGrey(r.m_male_trait);
  ui->female_preference->SetSurfaceGrey(r.m_female_preference);
}
