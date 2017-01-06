#include "elly_qtmaindialog.h"

#include <QFile>
#include <QMessageBox>
#include <cassert>
#include <chrono>
#include <qwt_legend.h>
#include <qwt_plot.h>
#include <qwt_point_data.h>
#include <qwt_plot_zoomer.h>
#include <qwt_text.h>
#include <qwt_plot_curve.h>
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
      m_plot_rates{new QwtPlot(QwtText("Rates"), this)},
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
  //Add legend
  {
    QwtLegend *const legend = new QwtLegend;
    legend->setFrameStyle(QFrame::Box | QFrame::Sunken);
    m_plot_rates->insertLegend(legend, QwtPlot::RightLegend);
  }

  assert(std::stod("0.005") > 0.004);
  this->set_parameters(create_parameters_set1());
  assert(get_parameters() == create_parameters_set1());

  //Add zoomer, must be done after the first plotting
  //{
  //  new QwtPlotZoomer(m_plot_rates);
  //}
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
  return v;
}

elly::per_species_rate elly::qtmaindialog::get_clad_is() const noexcept
{
  return ui->parameters->item(row_clado_is, 0)->text().toDouble();
}

elly::per_species_rate elly::qtmaindialog::get_clad_main() const noexcept
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
    get_clad_is(),
    get_clad_main(),
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

  const auto es = collect_all_events();
  assert(es.size() == 10);
  for (int i=0; i!=10; ++i)
  {
    const elly::event e{es[i]};
    const std::vector<double> ys = to_raw(collect(v, e));
    assert(xs.size() == ys.size());
    QwtPointArrayData *const data =
        new QwtPointArrayData(&xs[0], &ys[0], xs.size());
    m_curves_rates[i]->setData(data);
    m_curves_rates[i]->setTitle(QwtText(to_str(e).c_str()));
  }
  m_plot_rates->replot();
}

void elly::qtmaindialog::set_clad_is(const per_species_rate clado_is) noexcept
{
  ui->parameters->item(row_clado_is, 0)->setText(QString::number(clado_is.get()));
}

void elly::qtmaindialog::set_clad_main(const per_species_rate clado_main) noexcept
{
  assert(ui->parameters->rowCount() > row_clado_main);
  assert(ui);
  assert(ui->parameters);
  assert(ui->parameters->item(row_clado_main, 0));
  ui->parameters->item(row_clado_main, 0)->setText(QString::number(clado_main.get()));
}
void elly::qtmaindialog::set_ana(const per_species_rate ana) noexcept
{
  ui->parameters->item(row_ana, 0)->setText(QString::number(ana.get()));
}
void elly::qtmaindialog::set_ext_is(const per_species_rate ext_is) noexcept
{
  ui->parameters->item(row_ext_is, 0)->setText(QString::number(ext_is.get()));
}
void elly::qtmaindialog::set_ext_main(const per_species_rate ext_main) noexcept
{
  ui->parameters->item(row_ext_main, 0)->setText(QString::number(ext_main.get()));
}
void elly::qtmaindialog::set_mig_to_is(const per_species_rate mig_to_is) noexcept
{
  ui->parameters->item(row_mig_to_is, 0)->setText(QString::number(mig_to_is.get()));
}
void elly::qtmaindialog::set_carryingcap_is(const int carryingcap_is)
{
  ui->parameters->item(row_carryingcap_is, 0)->setText(QString::number(carryingcap_is));
}
void elly::qtmaindialog::set_carryingcap_main(const int carryingcap_main)
{
  ui->parameters->item(row_carryingcap_main, 0)->setText(QString::number(carryingcap_main));
}
void elly::qtmaindialog::set_rng_seed(const int rng_seed) noexcept
{
  ui->parameters->item(row_rng_seed, 0)->setText(QString::number(rng_seed));
}
void elly::qtmaindialog::set_init_n_mainland(const int init_n_mainland)
{
  ui->parameters->item(row_init_n_mainland, 0)->setText(QString::number(init_n_mainland));
}
void elly::qtmaindialog::set_crown_age(const double crown_age)
{
  ui->parameters->item(row_crown_age, 0)->setText(QString::number(crown_age));
}

void elly::qtmaindialog::set_parameters(const parameters &p) noexcept
{
  set_clad_is(p.get_clado_rate_is());
  set_clad_main(p.get_clado_rate_main());
  set_ana(p.get_ana_rate());
  set_ext_is(p.get_ext_rate_is());
  set_ext_main(p.get_ext_rate_main());
  set_mig_to_is(p.get_mig_rate_to_island());
  set_carryingcap_is(p.get_carryingcap_is());
  set_carryingcap_main(p.get_carryingcap_main());
  set_rng_seed(p.get_rng_seed());
  set_init_n_mainland(p.get_init_n_mainland());
  set_crown_age(p.get_crown_age());
  assert(get_parameters() == p);
}
