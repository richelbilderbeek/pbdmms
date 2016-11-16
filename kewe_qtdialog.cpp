#include "kewe_qtdialog.h"

#include <cassert>

#include<QFile>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_point_data.h>
#include <qwt_text.h>

#include "kewe_simulation.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "ui_kewe_qtdialog.h"
#pragma GCC diagnostic pop

std::vector<double> convert_to_vd(const std::vector<int>& v)
{
  std::vector<double> w;
  w.reserve(v.size());
  std::transform(
    std::begin(v),
    std::end(v),
    std::back_inserter(w),
    [](const int i) { return static_cast<double>(i); }
  );
  return w;
}

kewe::qtdialog::qtdialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::kewe_qtdialog),
  m_plot{new QwtPlot(QwtText("results"), this)},
  m_plot_lines{create_initial_plot_lines()}
{
  ui->setupUi(this);
  ui->checkBox->setChecked(true);

  assert(ui->results->layout());
  ui->results->layout()->addWidget(m_plot);

  m_plot->setMinimumHeight(400);
  for (const auto line: m_plot_lines)
  {
    line->attach(m_plot);
    line->setStyle(QwtPlotCurve::Steps);
    line->setPen(Qt::black, 2.0);
  }
}

kewe::qtdialog::~qtdialog()
{
  delete ui;
}

std::array<QwtPlotCurve *, 7> kewe::create_initial_plot_lines() noexcept
{
  std::array<QwtPlotCurve *, 7> v;
  for (auto& i: v) { i = new QwtPlotCurve; }
  return v;
}

kewe_parameters kewe::qtdialog::get_parameters() const
{
  QFile f(":/kewe/kewe_testparameters");
  f.copy("testparameters");
  kewe_parameters p = read_parameters("testparameters");

  p.sim_parameters.endtime = ui->parameters->item(0,0)->text().toInt();
  p.sim_parameters.popsize = ui->parameters->item(1,0)->text().toInt();
  p.sim_parameters.c = ui->parameters->item(2,0)->text().toDouble();
  p.sim_parameters.x0 = ui->parameters->item(3,0)->text().toDouble();
  p.sim_parameters.p0 = ui->parameters->item(4,0)->text().toDouble();
  p.sim_parameters.q0 = ui->parameters->item(5,0)->text().toDouble();
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

  plot_result_variables(s.get_output());
}

void kewe::qtdialog::on_checkBox_clicked()
{
  ui->checkBox_2->setChecked(!(ui->checkBox->isChecked()));
}

void kewe::qtdialog::on_checkBox_2_clicked()
{
  ui->checkBox->setChecked(!(ui->checkBox_2->isChecked()));
}

void kewe::qtdialog::plot_result_variables(const result_variables& r)
{
  const std::vector<double> xs = convert_to_vd(r.m_t);
  //0 : popsize
  {
    const std::vector<double>& ys = r.m_popsize;
    assert(xs.size() == ys.size());
    QwtPointArrayData * const data = new QwtPointArrayData(&xs[0],&ys[0],xs.size());
    m_plot_lines[0]->setData(data);
  }
  //1 : popsize
  {
    const std::vector<double>& ys = r.m_rhopq;
    assert(xs.size() == ys.size());
    QwtPointArrayData * const data = new QwtPointArrayData(&xs[0],&ys[0],xs.size());
    m_plot_lines[1]->setData(data);
  }

  m_plot->replot();

}
