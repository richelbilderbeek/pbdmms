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
  //on_start_clicked();
}

kewe::qtdialog::~qtdialog()
{
  delete ui;
}

kewe_parameters kewe::qtdialog::get_parameters() const
{
  kewe_parameters p;
  p.sim_parameters.seed = ui->parameters->item(0,0)->text().toInt();
  p.sim_parameters.endtime = ui->parameters->item(1,0)->text().toInt();
  p.sim_parameters.popsize = ui->parameters->item(2,0)->text().toInt();
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
