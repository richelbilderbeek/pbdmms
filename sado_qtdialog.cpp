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
  try
  {
    simulation s(get_parameters());
    s.run();
    const auto r = s.get_results();
    ui->eco_trait->SetSurfaceGrey(r.m_ecological_trait);
    ui->male_sexual_trait->SetSurfaceGrey(r.m_male_trait);
    ui->female_preference->SetSurfaceGrey(r.m_female_preference);
    plot_timeseries(s.get_results());
    this->setWindowTitle("");
  }
  catch (std::exception& e)
  {
    this->setWindowTitle(e.what());
  }
}

void sado::qtdialog::plot_timeseries(const results& r)
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

void sado::qtdialog::set_b(const double b) noexcept
{
  ui->parameters->item(0,0)->setText(QString::number(b));
}

void sado::qtdialog::set_c(const double c) noexcept
{
  ui->parameters->item(1,0)->setText(QString::number(c));
}

void sado::qtdialog::set_end_time(const int end_time) noexcept
{
  ui->parameters->item(2,0)->setText(QString::number(end_time));
}

void sado::qtdialog::set_erase_method(const erasure_method em) noexcept
{
  ui->parameters->item(3,0)->setText(QString::number(static_cast<int>(em)));
}

void sado::qtdialog::set_eta(const double eta) noexcept
{
  ui->parameters->item(4,0)->setText(QString::number(eta));
}

void sado::qtdialog::set_histbinp(const double histbinp) noexcept
{
  ui->parameters->item(5,0)->setText(QString::number(histbinp));
}

void sado::qtdialog::set_histbinq(const double histbinq) noexcept
{
  ui->parameters->item(6,0)->setText(QString::number(histbinq));
}

void sado::qtdialog::set_histbinx(const double histbinx) noexcept
{
  ui->parameters->item(7,0)->setText(QString::number(histbinx));
}

void sado::qtdialog::set_next_gen_method(const next_generation_method ngm) noexcept
{
  ui->parameters->item(8,0)->setText(QString::number(static_cast<int>(ngm)));
}

void sado::qtdialog::set_output_filename(const std::string& output_filename) noexcept
{
  ui->parameters->item(9,0)->setText(output_filename.c_str());
}

void sado::qtdialog::set_output_freq(const int output_freq) noexcept
{
  ui->parameters->item(10,0)->setText(QString::number(output_freq));
}

void sado::qtdialog::set_p0(const double p0) noexcept
{
  ui->parameters->item(11,0)->setText(QString::number(p0));
}

void sado::qtdialog::set_pop_size(const int pop_size) noexcept
{
  ui->parameters->item(12,0)->setText(QString::number(pop_size));
}

void sado::qtdialog::set_q0(const double q0) noexcept
{
  ui->parameters->item(13,0)->setText(QString::number(q0));
}

void sado::qtdialog::set_sc(const double sc) noexcept
{
  ui->parameters->item(14,0)->setText(QString::number(sc));
}

void sado::qtdialog::set_se(const double se) noexcept
{
  ui->parameters->item(15,0)->setText(QString::number(se));
}

void sado::qtdialog::set_seed(const int seed) noexcept
{
  ui->parameters->item(16,0)->setText(QString::number(seed));
}

void sado::qtdialog::set_sk(const double sk) noexcept
{
  ui->parameters->item(17,0)->setText(QString::number(sk));
}

void sado::qtdialog::set_sm(const double sm) noexcept
{
  ui->parameters->item(18,0)->setText(QString::number(sm));
}

void sado::qtdialog::set_sq(const double sq) noexcept
{
  ui->parameters->item(19,0)->setText(QString::number(sq));
}

void sado::qtdialog::set_sv(const double sv) noexcept
{
  ui->parameters->item(20,0)->setText(QString::number(sv));
}

void sado::qtdialog::set_use_initialization_bug(const bool use_initialization_bug) noexcept
{
  ui->parameters->item(21,0)->setText(QString::number(use_initialization_bug));
}

void sado::qtdialog::set_x0(const double x0) noexcept
{
  ui->parameters->item(22,0)->setText(QString::number(x0));
}

void sado::qtdialog::set_parameters(const parameters& p) noexcept
{
  set_b(p.get_b());
  set_c(p.get_c());
  set_end_time(p.get_end_time());
  set_erase_method(p.get_erasure());
  set_eta(p.get_eta());
  set_histbinp(p.get_histbinp());
  set_histbinq(p.get_histbinq());
  set_histbinx(p.get_histbinx());
  set_next_gen_method(p.get_next_gen_method());
  set_output_filename(p.get_output_filename());
  set_output_freq(p.get_output_freq());
  set_p0(p.get_p0());
  set_pop_size(p.get_pop_size());
  set_q0(p.get_q0());
  set_sc(p.get_sc());
  set_se(p.get_se());
  set_seed(p.get_seed());
  set_sk(p.get_sk());
  set_sm(p.get_sm());
  set_sq(p.get_sq());
  set_sv(p.get_sv());
  set_use_initialization_bug(p.get_use_initialization_bug());
  set_x0(p.get_x0());
}

