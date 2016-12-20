#include "kewe_qtdialog.h"

#include <cassert>

#include<QFile>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_point_data.h>
#include <qwt_text.h>
#include <qwt_legend.h>

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
  m_plot_lines[0]->setPen(QColor(255,0,0), 2.0);
  m_plot_lines[1]->setPen(QColor(0,255,0), 2.0);
  m_plot_lines[2]->setPen(QColor(0,0,255), 2.0);
  m_plot_lines[3]->setPen(QColor(128,128,0), 2.0);
  m_plot_lines[4]->setPen(QColor(128,0,128), 2.0);
  m_plot_lines[5]->setPen(QColor(0,128,128), 2.0);
  m_plot_lines[0]->setTitle(QwtText("Rho XP"));
  m_plot_lines[1]->setTitle(QwtText("Rho XQ"));
  m_plot_lines[2]->setTitle(QwtText("Rho PQ"));
  m_plot_lines[3]->setTitle(QwtText("SX"));
  m_plot_lines[4]->setTitle(QwtText("SP"));
  m_plot_lines[5]->setTitle(QwtText("SQ"));

  {
    QwtLegend * const legend = new QwtLegend;
    legend->setFrameStyle(QFrame::Box|QFrame::Sunken);
    m_plot->insertLegend(legend, QwtPlot::RightLegend);
  }

  ui->eco_trait->setMinimumHeight(400);
  ui->male_sexual_trait->setMinimumHeight(400);
  ui->female_preference->setMinimumHeight(400);
}

kewe::qtdialog::~qtdialog()
{
  delete ui;
}

std::array<QwtPlotCurve *, 6> kewe::create_initial_plot_lines() noexcept
{
  std::array<QwtPlotCurve *, 6> v;
  for (auto& i: v) { i = new QwtPlotCurve; }
  return v;
}

kewe::parameters kewe::qtdialog::get_parameters() const
{
  QFile f(":/kewe/kewe_testparameters");
  f.copy("testparameters");
  kewe::parameters p = read_parameters("testparameters");

  p.m_sim_parameters.endtime = ui->parameters->item(0,0)->text().toInt();
  p.m_sim_parameters.popsize = ui->parameters->item(1,0)->text().toInt();
  p.m_sim_parameters.c = ui->parameters->item(2,0)->text().toDouble();
  p.m_sim_parameters.x0 = ui->parameters->item(3,0)->text().toDouble();
  p.m_sim_parameters.p0 = ui->parameters->item(4,0)->text().toDouble();
  p.m_sim_parameters.q0 = ui->parameters->item(5,0)->text().toDouble();
  p.m_sim_parameters.haploid = static_cast<int>(ui->checkBox->isChecked());
  p.m_sim_parameters.diploid = static_cast<int>(ui->checkBox_2->isChecked());
  p.m_sim_parameters.sk = ui->parameters->item(6,0)->text().toDouble();
  p.m_sim_parameters.sc = ui->parameters->item(7,0)->text().toDouble();
  p.m_sim_parameters.se = ui->parameters->item(8,0)->text().toDouble();
  p.m_sim_parameters.sm = ui->parameters->item(9,0)->text().toDouble();
  p.m_sim_parameters.sv = ui->parameters->item(10,0)->text().toDouble();
  p.m_sim_parameters.at = ui->parameters->item(11,0)->text().toDouble();
  p.m_sim_parameters.sq = ui->parameters->item(12,0)->text().toDouble();

  return p;
}

void kewe::qtdialog::on_start_clicked()
{
  const kewe::parameters parameters = get_parameters();
  simulation s(parameters);
  s.run();
  const auto r = s.get_results();
  ui->eco_trait->SetSurfaceGrey(r.m_ecological_trait);
  ui->male_sexual_trait->SetSurfaceGrey(r.m_male_trait);
  ui->female_preference->SetSurfaceGrey(r.m_female_preference);

  //plot_result_variables(s.get_result_variables());
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

  //0 : rhoxp
  {
    const std::vector<double>& ys = r.m_rhoxp;
    assert(xs.size() == ys.size());
    QwtPointArrayData * const data = new QwtPointArrayData(&xs[0],&ys[0],xs.size());
    m_plot_lines[0]->setData(data);
  }
  //1 : rhoxq
  {
    const std::vector<double>& ys = r.m_rhoxq;
    assert(xs.size() == ys.size());
    QwtPointArrayData * const data = new QwtPointArrayData(&xs[0],&ys[0],xs.size());
    m_plot_lines[1]->setData(data);
  }
  //2 : rhopq
  {
    const std::vector<double>& ys = r.m_rhopq;
    assert(xs.size() == ys.size());
    QwtPointArrayData * const data = new QwtPointArrayData(&xs[0],&ys[0],xs.size());
    m_plot_lines[2]->setData(data);
  }
  //3 : sx
  {
    const std::vector<double>& ys = r.m_sx;
    assert(xs.size() == ys.size());
    QwtPointArrayData * const data = new QwtPointArrayData(&xs[0],&ys[0],xs.size());
    m_plot_lines[3]->setData(data);
  }
  //4 : sp
  {
    const std::vector<double>& ys = r.m_sp;
    assert(xs.size() == ys.size());
    QwtPointArrayData * const data = new QwtPointArrayData(&xs[0],&ys[0],xs.size());
    m_plot_lines[4]->setData(data);
  }
  //5 : sq
  {
    const std::vector<double>& ys = r.m_sq;
    assert(xs.size() == ys.size());
    QwtPointArrayData * const data = new QwtPointArrayData(&xs[0],&ys[0],xs.size());
    m_plot_lines[5]->setData(data);
  }

  m_plot->replot();

}
