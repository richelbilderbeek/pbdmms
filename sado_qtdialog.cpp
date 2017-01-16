#include "sado_qtdialog.h"

#include <QFile>
#include <QMessageBox>
#include <cassert>
#include <chrono>
#include <qwt_legend.h>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_point_data.h>
#include <qwt_text.h>
#include <sstream>

#include "sado_simulation.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "ui_sado_qtdialog.h"
#pragma GCC diagnostic pop

const int row_b{0};
const int row_c{row_b + 1};
const int row_end_time{row_c + 1};
const int row_eta{row_end_time + 1};
const int row_histbinp{row_eta + 1};
const int row_histbinq{row_histbinp + 1};
const int row_histbinx{row_histbinq + 1};
const int row_output_filename{row_histbinx + 1};
const int row_output_freq{row_output_filename + 1};
const int row_p0{row_output_freq + 1};
const int row_pop_size{row_p0 + 1};
const int row_q0{row_pop_size + 1};
const int row_sc{row_q0 + 1};
const int row_se{row_sc + 1};
const int row_seed{row_se + 1};
const int row_sk{row_seed + 1};
const int row_sm{row_sk + 1};
const int row_sq{row_sm + 1};
const int row_sv{row_sq + 1};
const int row_x0{row_sv + 1};

std::vector<double> convert_to_vd(const std::vector<int> &v)
{
  std::vector<double> w;
  w.reserve(v.size());
  std::transform(
      std::begin(v), std::end(v), std::back_inserter(w), [](const int i) {
        return static_cast<double>(i);
      });
  return w;
}

sado::qtdialog::qtdialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::sado_qtdialog),
      m_plot{new QwtPlot(QwtText("results"), this)},
      m_plot_lines{create_initial_plot_lines()}
{
  ui->setupUi(this);

  assert(ui->widget_right->layout());
  ui->widget_right->layout()->addWidget(m_plot);

  m_plot->setMinimumHeight(400);
  for (const auto line : m_plot_lines)
  {
    line->attach(m_plot);
    line->setStyle(QwtPlotCurve::Steps);
    line->setPen(Qt::black, 2.0);
  }
  m_plot_lines[0]->setPen(QColor(255, 0, 0), 2.0);
  m_plot_lines[1]->setPen(QColor(0, 255, 0), 2.0);
  m_plot_lines[2]->setPen(QColor(0, 0, 255), 2.0);
  m_plot_lines[3]->setPen(QColor(128, 128, 0), 2.0);
  m_plot_lines[4]->setPen(QColor(128, 0, 128), 2.0);
  m_plot_lines[5]->setPen(QColor(0, 128, 128), 2.0);
  m_plot_lines[0]->setTitle(QwtText("Rho XP"));
  m_plot_lines[1]->setTitle(QwtText("Rho XQ"));
  m_plot_lines[2]->setTitle(QwtText("Rho PQ"));
  m_plot_lines[3]->setTitle(QwtText("SX"));
  m_plot_lines[4]->setTitle(QwtText("SP"));
  m_plot_lines[5]->setTitle(QwtText("SQ"));

  {
    QwtLegend *const legend = new QwtLegend;
    legend->setFrameStyle(QFrame::Box | QFrame::Sunken);
    m_plot->insertLegend(legend, QwtPlot::RightLegend);
  }

  ui->eco_trait->setMinimumHeight(400);
  ui->male_sexual_trait->setMinimumHeight(400);
  ui->female_preference->setMinimumHeight(400);

  assert(std::stod("0.005") > 0.004);
  this->set_parameters(create_golden_standard_parameters());
  assert(get_parameters() == create_golden_standard_parameters());
}

sado::qtdialog::~qtdialog() { delete ui; }

std::array<QwtPlotCurve *, 6> sado::create_initial_plot_lines() noexcept
{
  std::array<QwtPlotCurve *, 6> v;
  for (auto &i : v)
  {
    i = new QwtPlotCurve;
  }
  return v;
}

double sado::qtdialog::get_b() const noexcept
{
  return ui->parameters->item(row_b, 0)->text().toDouble();
}
double sado::qtdialog::get_c() const noexcept
{
  return ui->parameters->item(row_c, 0)->text().toDouble();
}
int sado::qtdialog::get_end_time() const noexcept
{
  return ui->parameters->item(row_end_time, 0)->text().toInt();
}
sado::erasure_method sado::qtdialog::get_erase_method() const noexcept
{
  return to_erasure_method(ui->box_erasure_method->currentText().toStdString());
}
double sado::qtdialog::get_eta() const noexcept
{
  return ui->parameters->item(row_eta, 0)->text().toDouble();
}

sado::gausser_implementation sado::qtdialog::get_gausser_implementation() const
    noexcept
{
  return to_gausser_implementation(
      ui->box_gausser_implementation->currentText().toStdString());
}

double sado::qtdialog::get_histbinp() const noexcept
{
  return ui->parameters->item(row_histbinp, 0)->text().toDouble();
}

double sado::qtdialog::get_histbinq() const noexcept
{
  return ui->parameters->item(row_histbinq, 0)->text().toDouble();
}

double sado::qtdialog::get_histbinx() const noexcept
{
  return ui->parameters->item(row_histbinx, 0)->text().toDouble();
}

sado::next_generation_method sado::qtdialog::get_next_gen_method() const
    noexcept
{
  return to_next_generation_method(
      ui->box_next_generation_method->currentText().toStdString());
}

std::string sado::qtdialog::get_output_filename() const noexcept
{
  return ui->parameters->item(row_output_filename, 0)->text().toStdString();
}

int sado::qtdialog::get_output_freq() const noexcept
{
  return ui->parameters->item(row_output_freq, 0)->text().toInt();
}

double sado::qtdialog::get_p0() const noexcept
{
  return ui->parameters->item(row_p0, 0)->text().toDouble();
}

int sado::qtdialog::get_pop_size() const noexcept
{
  return ui->parameters->item(row_pop_size, 0)->text().toInt();
}

double sado::qtdialog::get_q0() const noexcept
{
  return ui->parameters->item(row_q0, 0)->text().toDouble();
}

double sado::qtdialog::get_sc() const noexcept
{
  return ui->parameters->item(row_sc, 0)->text().toDouble();
}

double sado::qtdialog::get_se() const noexcept
{
  return ui->parameters->item(row_se, 0)->text().toDouble();
}

int sado::qtdialog::get_seed() const noexcept
{
  return ui->parameters->item(row_seed, 0)->text().toInt();
}

double sado::qtdialog::get_sk() const noexcept
{
  return ui->parameters->item(row_sk, 0)->text().toDouble();
}

double sado::qtdialog::get_sm() const noexcept
{
  return ui->parameters->item(row_sm, 0)->text().toDouble();
}

double sado::qtdialog::get_sq() const noexcept
{
  return ui->parameters->item(row_sq, 0)->text().toDouble();
}

double sado::qtdialog::get_sv() const noexcept
{
  return ui->parameters->item(row_sv, 0)->text().toDouble();
}

bool sado::qtdialog::get_use_initialization_bug() const noexcept
{
  return ui->box_use_initialization_bug->isChecked();
}

double sado::qtdialog::get_x0() const noexcept
{
  return ui->parameters->item(row_x0, 0)->text().toDouble();
}

sado::parameters sado::qtdialog::get_parameters() const
{
  return parameters(
      get_b(),
      get_c(),
      get_end_time(),
      get_erase_method(),
      get_eta(),
      get_gausser_implementation(),
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
      get_x0());
}

void sado::qtdialog::on_start_clicked()
{
  try
  {
    using my_clock = std::chrono::high_resolution_clock;

    // get the clock time before operation.
    // note that this is a static function, and
    // we don't actually create a clock object
    const auto start_time = my_clock::now();

    const parameters p{get_parameters()};
    simulation s(p);
    ui->progressBar->setMaximum(p.get_end_time());
    for (int t{0}; t <= p.get_end_time();
         ++t) // Inclusive, as in original implementation
    {
      ui->progressBar->setValue(t);
      s.do_timestep();
    }
    const auto r = s.get_results();
    ui->eco_trait->SetSurfaceGrey(r.get_ecological_trait());
    ui->male_sexual_trait->SetSurfaceGrey(r.get_male_trait());
    ui->female_preference->SetSurfaceGrey(r.get_female_preference());
    plot_timeseries(s.get_results());
    this->setWindowTitle("");

    const auto end_time = my_clock::now();
    const auto diff = end_time - start_time;
    std::stringstream t;
    t << "Simulation lasted "
      << std::chrono::duration_cast<std::chrono::seconds>(diff).count()
      << " seconds";
    ui->label_sim_runtime->setText(t.str().c_str());
  }
  catch (std::exception &e)
  {
    this->setWindowTitle(e.what());
  }
}

void sado::qtdialog::plot_timeseries(const results &r)
{
  const std::vector<double> xs{convert_to_vd(r.collect_ts())};

  // 0 : rhoxp
  {
    const std::vector<double> ys = r.collect_rhoxps();
    assert(xs.size() == ys.size());
    QwtPointArrayData *const data =
        new QwtPointArrayData(&xs[0], &ys[0], xs.size());
    m_plot_lines[0]->setData(data);
  }
  // 1 : rhoxq
  {
    const std::vector<double> ys = r.collect_rhoxqs();
    assert(xs.size() == ys.size());
    QwtPointArrayData *const data =
        new QwtPointArrayData(&xs[0], &ys[0], xs.size());
    m_plot_lines[1]->setData(data);
  }
  // 2 : rhopq
  {
    const std::vector<double> ys = r.collect_rhopqs();
    assert(xs.size() == ys.size());
    QwtPointArrayData *const data =
        new QwtPointArrayData(&xs[0], &ys[0], xs.size());
    m_plot_lines[2]->setData(data);
  }
  // 3 : sx
  {
    const std::vector<double> ys = r.collect_sxs();
    assert(xs.size() == ys.size());
    QwtPointArrayData *const data =
        new QwtPointArrayData(&xs[0], &ys[0], xs.size());
    m_plot_lines[3]->setData(data);
  }
  // 4 : sp
  {
    const std::vector<double> ys = r.collect_sps();
    assert(xs.size() == ys.size());
    QwtPointArrayData *const data =
        new QwtPointArrayData(&xs[0], &ys[0], xs.size());
    m_plot_lines[4]->setData(data);
  }
  // 5 : sq
  {
    const std::vector<double> ys = r.collect_sqs();
    assert(xs.size() == ys.size());
    QwtPointArrayData *const data =
        new QwtPointArrayData(&xs[0], &ys[0], xs.size());
    m_plot_lines[5]->setData(data);
  }

  m_plot->replot();
}

void sado::qtdialog::set_b(const double b) noexcept
{
  ui->parameters->item(row_b, 0)->setText(QString::number(b));
}

void sado::qtdialog::set_c(const double c) noexcept
{
  ui->parameters->item(row_c, 0)->setText(QString::number(c));
}

void sado::qtdialog::set_end_time(const int end_time) noexcept
{
  ui->parameters->item(row_end_time, 0)->setText(QString::number(end_time));
}

void sado::qtdialog::set_erase_method(const erasure_method em) noexcept
{
  if (em == erasure_method::erase)
  {
    ui->box_erasure_method->setCurrentIndex(0);
  }
  else
  {
    assert(em == erasure_method::swap);
    ui->box_erasure_method->setCurrentIndex(1);
  }
  assert(get_erase_method() == em);
}

void sado::qtdialog::set_eta(const double eta) noexcept
{
  ui->parameters->item(row_eta, 0)->setText(QString::number(eta));
}

void sado::qtdialog::set_gausser_implementation(
    const gausser_implementation gi) noexcept
{
  if (gi == gausser_implementation::raw)
  {
    ui->box_gausser_implementation->setCurrentIndex(0);
  }
  else
  {
    assert(gi == gausser_implementation::lut);
    ui->box_gausser_implementation->setCurrentIndex(1);
  }
  assert(get_gausser_implementation() == gi);
}

void sado::qtdialog::set_histbinp(const double histbinp) noexcept
{
  ui->parameters->item(row_histbinp, 0)->setText(QString::number(histbinp));
}

void sado::qtdialog::set_histbinq(const double histbinq) noexcept
{
  ui->parameters->item(row_histbinq, 0)->setText(QString::number(histbinq));
}

void sado::qtdialog::set_histbinx(const double histbinx) noexcept
{
  ui->parameters->item(row_histbinx, 0)->setText(QString::number(histbinx));
}

void sado::qtdialog::set_next_gen_method(
    const next_generation_method ngm) noexcept
{
  if (ngm == next_generation_method::overlapping)
  {
    ui->box_next_generation_method->setCurrentIndex(0);
  }
  else
  {
    assert(ngm == next_generation_method::seperate);
    ui->box_next_generation_method->setCurrentIndex(1);
  }
  assert(get_next_gen_method() == ngm);
}

void sado::qtdialog::set_output_filename(
    const std::string &output_filename) noexcept
{
  ui->parameters->item(row_output_filename, 0)
      ->setText(output_filename.c_str());
}

void sado::qtdialog::set_output_freq(const int output_freq) noexcept
{
  ui->parameters->item(row_output_freq, 0)
      ->setText(QString::number(output_freq));
}

void sado::qtdialog::set_p0(const double p0) noexcept
{
  ui->parameters->item(row_p0, 0)->setText(QString::number(p0));
}

void sado::qtdialog::set_pop_size(const int pop_size) noexcept
{
  ui->parameters->item(row_pop_size, 0)->setText(QString::number(pop_size));
}

void sado::qtdialog::set_q0(const double q0) noexcept
{
  ui->parameters->item(row_q0, 0)->setText(QString::number(q0));
}

void sado::qtdialog::set_sc(const double sc) noexcept
{
  ui->parameters->item(row_sc, 0)->setText(QString::number(sc));
}

void sado::qtdialog::set_se(const double se) noexcept
{
  ui->parameters->item(row_se, 0)->setText(QString::number(se));
}

void sado::qtdialog::set_seed(const int seed) noexcept
{
  ui->parameters->item(row_seed, 0)->setText(QString::number(seed));
}

void sado::qtdialog::set_sk(const double sk) noexcept
{
  ui->parameters->item(row_sk, 0)->setText(QString::number(sk));
}

void sado::qtdialog::set_sm(const double sm) noexcept
{
  ui->parameters->item(row_sm, 0)->setText(QString::number(sm));
}

void sado::qtdialog::set_sq(const double sq) noexcept
{
  ui->parameters->item(row_sq, 0)->setText(QString::number(sq));
}

void sado::qtdialog::set_sv(const double sv) noexcept
{
  ui->parameters->item(row_sv, 0)->setText(QString::number(sv));
}

void sado::qtdialog::set_use_initialization_bug(
    const bool use_initialization_bug) noexcept
{
  ui->box_use_initialization_bug->setChecked(use_initialization_bug);
  assert(get_use_initialization_bug() == use_initialization_bug);
}

void sado::qtdialog::set_x0(const double x0) noexcept
{
  ui->parameters->item(row_x0, 0)->setText(QString::number(x0));
}

void sado::qtdialog::set_parameters(const parameters &p) noexcept
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

void sado::qtdialog::showEvent(QShowEvent *)
{
  const int h{
      (ui->widget_right->height() - ui->label_ecological_trait->height()) / 2};
  ui->male_sexual_trait->setMaximumHeight(h);
  ui->female_preference->setMaximumHeight(h);
  ui->eco_trait->setMaximumHeight(h);
  m_plot->setMaximumHeight(h);

  const int w{(ui->widget_center_right->width() + ui->widget_right->width()) /
              2};
  ui->widget_center_right->setMaximumWidth(w);
  ui->widget_right->setMaximumWidth(w);
}

void sado::qtdialog::on_button_view_parameters_clicked()
{
  const auto p = get_parameters();
  std::stringstream s;
  s << p;
  QMessageBox b;
  b.setWindowTitle("Just copy-paste this to a file:");
  b.setText(s.str().c_str());
  b.exec();
}
