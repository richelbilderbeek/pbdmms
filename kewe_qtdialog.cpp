
#include<QFile>
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
  ui->checkBox->setChecked(true);
  //on_start_clicked();
}

kewe::qtdialog::~qtdialog()
{
  delete ui;
}

kewe_parameters kewe::qtdialog::get_parameters() const
{
  QFile f(":/kewe/kewe_testparameters");
  f.copy("testparameters");
  kewe_parameters p = read_parameters("testparameters");

  p.sim_parameters.endtime = ui->parameters->item(0,0)->text().toInt();
  p.sim_parameters.popsize = ui->parameters->item(1,0)->text().toInt();
  p.sim_parameters.c = ui->parameters->item(2,0)->text().toDouble();
  p.sim_parameters.haploid = static_cast<int>(ui->checkBox->isChecked());
  p.sim_parameters.diploid = static_cast<int>(ui->checkBox_2->isChecked());
  return p;
}

void kewe::qtdialog::on_start_clicked()
{
  const kewe_parameters parameters = get_parameters();
  simulation s(parameters);
  s.run();
  const auto r = s.get_results();
  ui->eco_trait->SetSurfaceGrey(r.m_ecological_trait);
  ui->male_sexual_trait->SetSurfaceGrey(r.m_male_trait);
  ui->female_preference->SetSurfaceGrey(r.m_female_preference);
  //plot_result_variables();
}

/*

  std::vector<double> xs;
  std::vector<double> ys;
  for (const auto p: points)
  {
    xs.push_back(p.first);
    ys.push_back(p.second);
  }
  QwtPointArrayData * const data = new QwtPointArrayData(&xs[0],&ys[0],xs.size());
  m_nltt_plot_line->setData(data);
  m_nltt_plot->replot();
*/

void kewe::qtdialog::on_checkBox_clicked()
{
    ui->checkBox_2->setChecked(!(ui->checkBox->isChecked()));
}

void kewe::qtdialog::on_checkBox_2_clicked()
{
  ui->checkBox->setChecked(!(ui->checkBox_2->isChecked()));
}
