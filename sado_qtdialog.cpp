#include "sado_qtdialog.h"

#include <cassert>

#include<QFile>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_point_data.h>
#include <qwt_text.h>
#include <qwt_legend.h>

#include "sado_simulation.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "ui_sado_qtdialog.h"
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

sado::qtdialog::qtdialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::sado_qtdialog),
  m_plot{new QwtPlot(QwtText("results"), this)},
  m_plot_lines{create_initial_plot_lines()}
{
  ui->setupUi(this);

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

sado::qtdialog::~qtdialog()
{
  delete ui;
}

std::array<QwtPlotCurve *, 6> sado::create_initial_plot_lines() noexcept
{
  std::array<QwtPlotCurve *, 6> v;
  for (auto& i: v) { i = new QwtPlotCurve; }
  return v;
}

double sado::qtdialog::get_b() const noexcept
{
  return ui->parameters->item(0,0)->text().toDouble();
}
double sado::qtdialog::get_c() const noexcept
{
  return ui->parameters->item(1,0)->text().toDouble();
}
int sado::qtdialog::get_end_time() const noexcept
{
  return ui->parameters->item(2,0)->text().toInt();
}
sado::erasure_method sado::qtdialog::get_erase_method() const noexcept
{
  return static_cast<erasure_method>(ui->parameters->item(3,0)->text().toInt());
}
double sado::qtdialog::get_eta() const noexcept
{
  return ui->parameters->item(4,0)->text().toDouble();
}
double sado::qtdialog::get_histbinp() const noexcept
{
  return ui->parameters->item(5,0)->text().toDouble();
}
double sado::qtdialog::get_histbinq() const noexcept
{
  return ui->parameters->item(6,0)->text().toDouble();
}
double sado::qtdialog::get_histbinx() const noexcept
{
  return ui->parameters->item(7,0)->text().toDouble();
}
sado::next_generation_method sado::qtdialog::get_next_gen_method() const noexcept
{
  return static_cast<next_generation_method>(ui->parameters->item(8,0)->text().toInt());
}
std::string sado::qtdialog::get_output_filename() const noexcept
{
  return ui->parameters->item(9,0)->text().toStdString();
}
int sado::qtdialog::get_output_freq() const noexcept
{
  return ui->parameters->item(10,0)->text().toInt();
}
double sado::qtdialog::get_p0() const noexcept
{
  return ui->parameters->item(11,0)->text().toDouble();
}
int sado::qtdialog::get_pop_size() const noexcept
{
  return ui->parameters->item(12,0)->text().toInt();
}
double sado::qtdialog::get_q0() const noexcept
{
  return ui->parameters->item(13,0)->text().toDouble();
}
double sado::qtdialog::get_sc() const noexcept
{
  return ui->parameters->item(14,0)->text().toDouble();
}
double sado::qtdialog::get_se() const noexcept
{
  return ui->parameters->item(15,0)->text().toDouble();
}
int sado::qtdialog::get_seed() const noexcept
{
  return ui->parameters->item(16,0)->text().toInt();
}
double sado::qtdialog::get_sk() const noexcept
{
  return ui->parameters->item(17,0)->text().toDouble();
}
double sado::qtdialog::get_sm() const noexcept
{
  return ui->parameters->item(18,0)->text().toDouble();
}
double sado::qtdialog::get_sq() const noexcept
{
  return ui->parameters->item(19,0)->text().toDouble();
}
double sado::qtdialog::get_sv() const noexcept
{
  return ui->parameters->item(20,0)->text().toDouble();
}
bool sado::qtdialog::get_use_initialization_bug() const noexcept
{
  return ui->parameters->item(21,0)->text().toInt();
}
double sado::qtdialog::get_x0() const noexcept
{
  return ui->parameters->item(22,0)->text().toDouble();
}

sado::parameters sado::qtdialog::get_parameters() const
{
  return parameters(
    get_b(),
    get_c(),
    get_end_time(),
    get_erase_method(),
    get_eta(),
    get_histbinp(),
    get_histbinq(),
    get_histbinx(),
    get_next_gen_method(),
    get_output_filename(),
    get_output_freq(),
    get_p0(),
    get_pop_size(),
    get_q0(),
    get_sc(),
    get_se(),
    get_seed(),
    get_sk(),
    get_sm(),
    get_sq(),
    get_sv(),
    get_use_initialization_bug(),
    get_x0()
  );
}

void sado::qtdialog::on_start_clicked()
{
  simulation s(get_parameters());
  s.run();
  const auto r = s.get_results();
  ui->eco_trait->SetSurfaceGrey(r.m_ecological_trait);
  ui->male_sexual_trait->SetSurfaceGrey(r.m_male_trait);
  ui->female_preference->SetSurfaceGrey(r.m_female_preference);

  //plot_result_variables(s.get_result_variables());
}

void sado::qtdialog::plot_result_variables(const result_variables& r)
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

void sado::qtdialog::on_set_branching_clicked()
{
  const auto p = create_sim_parameters_branching();
  this->set_parameters(p);
  assert(p == this->get_parameters());
}

void sado::qtdialog::set_eco_res_distr_width(const double eco_res_distribution_width)
{
  ui->parameters->item(6,0)->setText(
    QString::number(eco_res_distribution_width)
  );
}

void sado::qtdialog::set_eco_res_util_width(const double eco_res_util_width)
{
  ui->parameters->item(7,0)->setText(
    QString::number(eco_res_util_width)
  );
}

void sado::qtdialog::set_initial_eco_trait(const double initial_eco_trait) noexcept
{
  ui->parameters->item(3,0)->setText(
    QString::number(initial_eco_trait)
  );
}

void sado::qtdialog::set_initial_fem_pref(const double initial_fem_pref) noexcept
{
  ui->parameters->item(4,0)->setText(
    QString::number(initial_fem_pref)
  );
}

void sado::qtdialog::set_initial_male_trait(const double initial_male_trait) noexcept
{
  ui->parameters->item(5,0)->setText(
    QString::number(initial_male_trait)
  );
}

void sado::qtdialog::set_mate_spec_eco(const double mate_spec_eco)
{
  ui->parameters->item(8,0)->setText(
    QString::number(mate_spec_eco)
  );
}

void sado::qtdialog::set_mate_spec_mate(const double mate_spec_mate)
{
  ui->parameters->item(9,0)->setText(
    QString::number(mate_spec_mate)
  );
}

void sado::qtdialog::set_mut_distr_width(const double mut_distr_width)
{
  ui->parameters->item(10,0)->setText(
    QString::number(mut_distr_width)
  );
}

void sado::qtdialog::set_parameters(const simulation_parameters& p) noexcept
{
  this->set_eco_res_distr_width(p.get_eco_res_distribution_width());
  this->set_eco_res_util_width(p.get_eco_res_util_width());
  this->set_mate_spec_eco(p.get_mate_spec_eco());
  this->set_mate_spec_mate(p.get_mate_spec_mate());
  this->set_mut_distr_width(p.get_mut_distr_width());
  this->set_viab_male_mate_str(p.get_viab_sel_male_mate_str());
}

void sado::qtdialog::set_viab_male_mate_str(const double viab_sel_male_mate_str)
{
  ui->parameters->item(12,0)->setText(
    QString::number(viab_sel_male_mate_str)
  );
}

void sado::qtdialog::on_show_branching_clicked()
{
  parameters p(
    output_parameters(),
    create_sim_parameters_branching()
  );
  p.m_output_parameters.is_silent = true;

  simulation s(p);
  s.run();
  const auto r = s.get_results();
  ui->eco_trait->SetSurfaceGrey(r.m_ecological_trait);
  ui->male_sexual_trait->SetSurfaceGrey(r.m_male_trait);
  ui->female_preference->SetSurfaceGrey(r.m_female_preference);
  //plot_result_variables(s.get_result_variables());

}
