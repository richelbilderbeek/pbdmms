#include "elly_qtmaindialog.h"

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

#include "elly_simulation.h"
#include "elly_events_rates_in_time.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "ui_elly_qtmaindialog.h"
#pragma GCC diagnostic pop

const int row_clado_is{0};
const int row_clado_main{row_clado_is + 1};
const int row_ana{row_clado_main + 1};
const int row_ext_is{row_ana + 1};
const int row_ext_main{row_ext_is + 1};
const int row_mig_to_is{row_ext_main + 1};
const int row_carryingcap_is{row_mig_to_is + 1};
const int row_carryingcap_main{row_carryingcap_is + 1};
const int row_rng_seed{row_carryingcap_main + 1};
const int row_init_n_mainland{row_rng_seed + 1};
const int row_crown_age{row_init_n_mainland + 1};

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

elly::qtmaindialog::qtmaindialog(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::elly_qtmaindialog),
      m_plot_rates{new QwtPlot(QwtText("results"), this)},
      m_curves_rates{create_initial_curves_rates()}
{
  ui->setupUi(this);

  assert(ui->widget_right->layout());
  ui->widget_right->layout()->addWidget(m_plot_rates);

  m_plot_rates->setMinimumHeight(400);
  for (const auto line : m_curves_rates)
  {
    line->attach(m_plot_rates);
  }
  {
    QwtLegend *const legend = new QwtLegend;
    legend->setFrameStyle(QFrame::Box | QFrame::Sunken);
    m_plot_rates->insertLegend(legend, QwtPlot::RightLegend);
  }

  assert(std::stod("0.005") > 0.004);
  this->set_parameters(create_parameters_set1());
  assert(get_parameters() == create_parameters_set1());
}

elly::qtmaindialog::~qtmaindialog() { delete ui; }

std::array<QwtPlotCurve *, 10> elly::create_initial_curves_rates() noexcept
{
  std::array<QwtPlotCurve *, 10> v;
  for (auto &i : v)
  {
    i = new QwtPlotCurve;
  }
  v[0]->setPen(QColor(  0,   0,   0), 2.0);

  v[1]->setPen(QColor(255, 128,   0), 2.0);
  v[2]->setPen(QColor(255, 255,   0), 2.0);
  v[3]->setPen(QColor(255,   0, 128), 2.0);
  v[4]->setPen(QColor(255,   0, 255), 2.0);

  v[5]->setPen(QColor(  0, 255, 128), 2.0);
  v[6]->setPen(QColor(  0, 255, 255), 2.0);
  v[7]->setPen(QColor(128, 255,   0), 2.0);
  v[8]->setPen(QColor(255, 255,   0), 2.0);

  v[9]->setPen(QColor(255, 255, 255), 2.0);

  v[0]->setTitle(QwtText("ana"));
  v[1]->setTitle(QwtText("clad_glob_on_island"));
  v[2]->setTitle(QwtText("clad_glob_on_main"));
  v[3]->setTitle(QwtText("clad_island_only"));
  v[4]->setTitle(QwtText("clad_main_only"));
  v[5]->setTitle(QwtText("ext_glob_on_island"));
  v[6]->setTitle(QwtText("ext_glob_on_main"));
  v[7]->setTitle(QwtText("ext_island_only"));
  v[8]->setTitle(QwtText("ext_main_only"));
  v[9]->setTitle(QwtText("migration_to_island"));
  return v;
}

elly::per_species_rate elly::qtmaindialog::get_clado_is() const noexcept
{
  return ui->parameters->item(row_clado_is, 0)->text().toDouble();
}

elly::per_species_rate elly::qtmaindialog::get_clado_main() const noexcept
{
  return ui->parameters->item(row_clado_main, 0)->text().toDouble();
}

elly::per_species_rate elly::qtmaindialog::get_ana() const noexcept
{
  return ui->parameters->item(row_ana, 0)->text().toDouble();
}

elly::per_species_rate elly::qtmaindialog::get_ext_is() const noexcept
{
  return ui->parameters->item(row_ext_is, 0)->text().toDouble();
}

elly::per_species_rate elly::qtmaindialog::get_ext_main() const noexcept
{
  return ui->parameters->item(row_ext_main, 0)->text().toDouble();
}

elly::per_species_rate elly::qtmaindialog::get_mig_to_is() const noexcept
{
  return ui->parameters->item(row_mig_to_is, 0)->text().toDouble();
}

int elly::qtmaindialog::get_carryingcap_is() const noexcept
{
  return ui->parameters->item(row_carryingcap_is, 0)->text().toDouble();
}

int elly::qtmaindialog::get_carryingcap_main() const noexcept
{
  return ui->parameters->item(row_carryingcap_main, 0)->text().toDouble();
}

int elly::qtmaindialog::get_rng_seed() const noexcept
{
  return ui->parameters->item(row_rng_seed, 0)->text().toDouble();
}

int elly::qtmaindialog::get_init_n_mainland() const noexcept
{
  return ui->parameters->item(row_init_n_mainland, 0)->text().toDouble();
}

double elly::qtmaindialog::get_crown_age() const noexcept
{
  return ui->parameters->item(row_crown_age, 0)->text().toDouble();
}


elly::parameters elly::qtmaindialog::get_parameters() const
{
  return parameters(
    get_clado_is(),
    get_clado_main(),
    get_ana(),
    get_ext_is(),
    get_ext_main(),
    get_mig_to_is(),
    get_carryingcap_is(),
    get_carryingcap_main(),
    get_rng_seed(),
    get_init_n_mainland(),
    get_crown_age()
  );
}

void elly::qtmaindialog::on_start_clicked()
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

    ui->progressBar->setMaximum(1000);
    while (s.get_time() <= p.get_crown_age())
    {
      const int progress{static_cast<int>(1000.0 * s.get_time()/p.get_crown_age())};
      ui->progressBar->setValue(s.get_time());
      s.do_next_event();
    }

    const auto r = s.get_event_rates();
    plot_event_rates(r);
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

void elly::qtmaindialog::plot_event_rates(
  const events_rates_in_time& v
)
{
  const std::vector<double> xs{collect_ts(v)};


  // 0 : ana
  {
    const std::vector<double> ys = collect(elly::event::ana, v);
    assert(xs.size() == ys.size());
    QwtPointArrayData *const data =
        new QwtPointArrayData(&xs[0], &ys[0], xs.size());
    m_curves_rates[0]->setData(data);
  }

  /*
  v[0]->setTitle(QwtText("ana"));
  v[1]->setTitle(QwtText("clad_glob_on_island"));
  v[2]->setTitle(QwtText("clad_glob_on_main"));
  v[3]->setTitle(QwtText("clad_island_only"));
  v[4]->setTitle(QwtText("clad_main_only"));
  v[5]->setTitle(QwtText("ext_glob_on_island"));
  v[6]->setTitle(QwtText("ext_glob_on_main"));
  v[7]->setTitle(QwtText("ext_island_only"));
  v[8]->setTitle(QwtText("ext_main_only"));
  v[9]->setTitle(QwtText("migration_to_island"));
  */

  // 1 : rhoxq
  {
    const std::vector<double> ys = r.collect_rhoxqs();
    assert(xs.size() == ys.size());
    QwtPointArrayData *const data =
        new QwtPointArrayData(&xs[0], &ys[0], xs.size());
    m_curves_rates[1]->setData(data);
  }
  // 2 : rhopq
  {
    const std::vector<double> ys = r.collect_rhopqs();
    assert(xs.size() == ys.size());
    QwtPointArrayData *const data =
        new QwtPointArrayData(&xs[0], &ys[0], xs.size());
    m_curves_rates[2]->setData(data);
  }
  // 3 : sx
  {
    const std::vector<double> ys = r.collect_sxs();
    assert(xs.size() == ys.size());
    QwtPointArrayData *const data =
        new QwtPointArrayData(&xs[0], &ys[0], xs.size());
    m_curves_rates[3]->setData(data);
  }
  // 4 : sp
  {
    const std::vector<double> ys = r.collect_sps();
    assert(xs.size() == ys.size());
    QwtPointArrayData *const data =
        new QwtPointArrayData(&xs[0], &ys[0], xs.size());
    m_curves_rates[4]->setData(data);
  }
  // 5 : sq
  {
    const std::vector<double> ys = r.collect_sqs();
    assert(xs.size() == ys.size());
    QwtPointArrayData *const data =
        new QwtPointArrayData(&xs[0], &ys[0], xs.size());
    m_curves_rates[5]->setData(data);
  }

  m_plot_rates->replot();
}

void elly::qtmaindialog::set_b(const double b) noexcept
{
  ui->parameters->item(row_b, 0)->setText(QString::number(b));
}

void elly::qtmaindialog::set_c(const double c) noexcept
{
  ui->parameters->item(row_c, 0)->setText(QString::number(c));
}

void elly::qtmaindialog::set_end_time(const int end_time) noexcept
{
  ui->parameters->item(row_end_time, 0)->setText(QString::number(end_time));
}

void elly::qtmaindialog::set_eta(const double eta) noexcept
{
  ui->parameters->item(row_eta, 0)->setText(QString::number(eta));
}

void elly::qtmaindialog::set_histbinp(const double histbinp) noexcept
{
  ui->parameters->item(row_histbinp, 0)->setText(QString::number(histbinp));
}

void elly::qtmaindialog::set_histbinq(const double histbinq) noexcept
{
  ui->parameters->item(row_histbinq, 0)->setText(QString::number(histbinq));
}

void elly::qtmaindialog::set_histbinx(const double histbinx) noexcept
{
  ui->parameters->item(row_histbinx, 0)->setText(QString::number(histbinx));
}

void elly::qtmaindialog::set_output_filename(
    const std::string &output_filename) noexcept
{
  ui->parameters->item(row_output_filename, 0)
      ->setText(output_filename.c_str());
}

void elly::qtmaindialog::set_output_freq(const int output_freq) noexcept
{
  ui->parameters->item(row_output_freq, 0)
      ->setText(QString::number(output_freq));
}

void elly::qtmaindialog::set_p0(const double p0) noexcept
{
  ui->parameters->item(row_p0, 0)->setText(QString::number(p0));
}

void elly::qtmaindialog::set_pop_size(const int pop_size) noexcept
{
  ui->parameters->item(row_pop_size, 0)->setText(QString::number(pop_size));
}

void elly::qtmaindialog::set_q0(const double q0) noexcept
{
  ui->parameters->item(row_q0, 0)->setText(QString::number(q0));
}

void elly::qtmaindialog::set_sc(const double sc) noexcept
{
  ui->parameters->item(row_sc, 0)->setText(QString::number(sc));
}

void elly::qtmaindialog::set_se(const double se) noexcept
{
  ui->parameters->item(row_se, 0)->setText(QString::number(se));
}

void elly::qtmaindialog::set_seed(const int seed) noexcept
{
  ui->parameters->item(row_seed, 0)->setText(QString::number(seed));
}

void elly::qtmaindialog::set_sk(const double sk) noexcept
{
  ui->parameters->item(row_sk, 0)->setText(QString::number(sk));
}

void elly::qtmaindialog::set_sm(const double sm) noexcept
{
  ui->parameters->item(row_sm, 0)->setText(QString::number(sm));
}

void elly::qtmaindialog::set_sq(const double sq) noexcept
{
  ui->parameters->item(row_sq, 0)->setText(QString::number(sq));
}

void elly::qtmaindialog::set_sv(const double sv) noexcept
{
  ui->parameters->item(row_sv, 0)->setText(QString::number(sv));
}

void elly::qtmaindialog::set_use_initialization_bug(
    const bool use_initialization_bug) noexcept
{
  ui->box_use_initialization_bug->setChecked(use_initialization_bug);
  assert(get_use_initialization_bug() == use_initialization_bug);
}

void elly::qtmaindialog::set_x0(const double x0) noexcept
{
  ui->parameters->item(row_x0, 0)->setText(QString::number(x0));
}

void elly::qtmaindialog::set_parameters(const parameters &p) noexcept
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

void elly::qtmaindialog::showEvent(QShowEvent *)
{
  const int h{
      (ui->widget_right->height() - ui->label_ecological_trait->height()) / 2};
  ui->male_sexual_trait->setMaximumHeight(h);
  ui->female_preference->setMaximumHeight(h);
  ui->eco_trait->setMaximumHeight(h);
  m_plot_rates->setMaximumHeight(h);

  const int w{(ui->widget_center_right->width() + ui->widget_right->width()) /
              2};
  ui->widget_center_right->setMaximumWidth(w);
  ui->widget_right->setMaximumWidth(w);
}

void elly::qtmaindialog::on_button_view_parameters_clicked()
{
  const auto p = get_parameters();
  std::stringstream s;
  s << p;
  QMessageBox b;
  b.setWindowTitle("Just copy-paste this to a file:");
  b.setText(s.str().c_str());
  b.exec();
}
