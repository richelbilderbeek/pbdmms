#include "elly_qtmaindialog.h"

#include <QFile>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QVBoxLayout>
#include <cassert>
#include <chrono>
#include <qwt_legend.h>
#include <qwt_plot.h>
#include <qwt_point_data.h>
#include <qwt_plot_zoomer.h>
#include <qwt_text.h>
#include <qwt_plot_curve.h>
#include <sstream>

#include "elly_events.h"
#include "elly_simulation.h"
#include "elly_location.h"
#include "elly_results.h"

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
const int row_init_n_mainland_sps{row_rng_seed + 1};
const int row_crown_age{row_init_n_mainland_sps + 1};

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
      m_curves_pop_sizes{create_initial_curves_pop_sizes()},
      m_curves_rates{create_initial_curves_rates()},
      m_daic_input{new QPlainTextEdit},
      m_plot_pop_sizes{new QwtPlot(QwtText("Population sizes"), this)},
      m_plot_rates{new QwtPlot(QwtText("Rates"), this)},
      m_sim_results{new QPlainTextEdit}
{
  ui->setupUi(this);

  add_widgets_to_ui();
  setup_widgets();
  attach_curves_to_plots();
  add_legends();
  //Set the standard testing parameters
  assert(std::stod("0.005") > 0.004); //Must be English
  on_button_2_clicked();
}

elly::qtmaindialog::~qtmaindialog() { delete ui; }

std::array<QwtPlotCurve *, 6> elly::create_initial_curves_pop_sizes() noexcept
{
  std::array<QwtPlotCurve *, 6> v;
  for (auto &i : v)
  {
    i = new QwtPlotCurve;
  }
  v[0]->setPen(QColor(255, 255,   0), 2.0); //mainland: yellow
  v[1]->setPen(QColor(  0, 255,   0), 2.0); //mainland-only: green
  v[2]->setPen(QColor(  0, 225, 255), 2.0); //island: cyan
  v[3]->setPen(QColor(  0,   0, 255), 2.0); //island-only: blue
  v[4]->setPen(QColor(255,   0,   0), 2.0); //both: red
  v[5]->setPen(QColor(  0,   0,   0), 2.0); //extinct: black
  return v;
}

void elly::qtmaindialog::add_legends() noexcept
{
  {
    QwtLegend *const legend = new QwtLegend;
    legend->setFrameStyle(QFrame::Box | QFrame::Sunken);
    m_plot_pop_sizes->insertLegend(legend, QwtPlot::RightLegend);
  }
  {
    QwtLegend *const legend = new QwtLegend;
    legend->setFrameStyle(QFrame::Box | QFrame::Sunken);
    m_plot_rates->insertLegend(legend, QwtPlot::RightLegend);
  }
}

void elly::qtmaindialog::add_widgets_to_ui() noexcept
{
  assert(!ui->widget_right->layout());
  ui->widget_right->setLayout(new QVBoxLayout);
  assert(ui->widget_right->layout());
  ui->widget_right->layout()->addWidget(m_plot_pop_sizes);
  ui->widget_right->layout()->addWidget(m_plot_rates);
  ui->widget_right->layout()->addWidget(m_sim_results);
  ui->widget_right->layout()->addWidget(m_daic_input);
}

void elly::qtmaindialog::attach_curves_to_plots() noexcept
{
  for (const auto line: m_curves_pop_sizes)
  {
    line->attach(m_plot_pop_sizes);
  }
  for (const auto line: m_curves_rates)
  {
    line->attach(m_plot_rates);
  }
}

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

elly::per_species_rate elly::qtmaindialog::get_clad_is() const
{
  return ui->parameters->item(row_clado_is, 0)->text().toDouble();
}

elly::per_species_rate elly::qtmaindialog::get_clad_main() const
{
  return ui->parameters->item(row_clado_main, 0)->text().toDouble();
}

elly::per_species_rate elly::qtmaindialog::get_ana() const
{
  return ui->parameters->item(row_ana, 0)->text().toDouble();
}

elly::per_species_rate elly::qtmaindialog::get_ext_is() const
{
  return ui->parameters->item(row_ext_is, 0)->text().toDouble();
}

elly::per_species_rate elly::qtmaindialog::get_ext_main() const
{
  return ui->parameters->item(row_ext_main, 0)->text().toDouble();
}

elly::per_species_rate elly::qtmaindialog::get_mig_to_is() const
{
  return ui->parameters->item(row_mig_to_is, 0)->text().toDouble();
}

int elly::qtmaindialog::get_carryingcap_is() const
{
  return ui->parameters->item(row_carryingcap_is, 0)->text().toDouble();
}

int elly::qtmaindialog::get_carryingcap_main() const
{
  return ui->parameters->item(row_carryingcap_main, 0)->text().toDouble();
}

int elly::qtmaindialog::get_rng_seed() const
{
  return ui->parameters->item(row_rng_seed, 0)->text().toDouble();
}

int elly::qtmaindialog::get_init_n_mainland_sps() const
{
  return ui->parameters->item(row_init_n_mainland_sps, 0)->text().toDouble();
}

double elly::qtmaindialog::get_crown_age() const
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
    get_init_n_mainland_sps(),
    get_crown_age()
  );
}

void elly::qtmaindialog::on_start_clicked()
{
  this->setWindowTitle(" ");
  try
  {
    using my_clock = std::chrono::high_resolution_clock;

    // get the clock time before operation.
    // note that this is a static function, and
    // we don't actually create a clock object
    const auto start_time = my_clock::now();

    const parameters p{get_parameters()};
    simulation s(p);

    ui->progress_bar->setMaximum(1000);
    while (s.get_time() <= p.get_crown_age())
    {
      const int progress{static_cast<int>(1000.0 * s.get_time()/p.get_crown_age())};
      ui->progress_bar->setValue(progress);
      s.do_next_event();
    }
    const auto measurements = s.get_measurements();

    plot_pop_sizes(measurements);
    plot_event_rates(measurements);
    plot_daic_input(get_results(s));
    plot_sim_results(get_results(s));

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
  ui->progress_bar->setValue(ui->progress_bar->maximum());
}

void elly::qtmaindialog::plot_daic_input(const results& v)
{
  const daic::input i_ideal = convert_ideal(v);
  const daic::input i_reality = convert_reality(v);
  std::stringstream s;
  s
    << "Ideal" << '\n'
    << "-----" << '\n'
    << i_ideal << '\n'
    << "Reality" << '\n'
    << "-------" << '\n'
    << i_reality << '\n'
  ;
  m_daic_input->setPlainText(s.str().c_str());
}

void elly::qtmaindialog::plot_event_rates(
  const measurements& v
)
{
  const std::vector<double> xs{collect_ts(v)};

  const std::vector<elly::event> es = collect_all_events();
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

void elly::qtmaindialog::plot_pop_sizes(
  const measurements& v
)
{
  const std::vector<double> xs{collect_ts(v)};

  const std::vector<location> locations = collect_all_locations();
  assert(locations.size() == 5);
  //Species at locations
  for (int i=0; i!=5; ++i)
  {
    const elly::location where{locations[i]};
    const std::vector<double> ys = convert_to_vd(collect(v, where));
    assert(xs.size() == ys.size());
    QwtPointArrayData *const data =
        new QwtPointArrayData(&xs[0], &ys[0], xs.size());
    m_curves_pop_sizes[i]->setData(data);
    m_curves_pop_sizes[i]->setTitle(QwtText(to_str(where).c_str()));
  }
  //Extinct species
  {
    const std::vector<double> ys = convert_to_vd(collect_extinct(v));
    assert(xs.size() == ys.size());
    QwtPointArrayData *const data =
        new QwtPointArrayData(&xs[0], &ys[0], xs.size());
    m_curves_pop_sizes[5]->setData(data);
    m_curves_pop_sizes[5]->setTitle(QwtText("Extinct"));
  }
  m_plot_pop_sizes->replot();
}

void elly::qtmaindialog::plot_sim_results(const results& v)
{
  std::stringstream s;
  s << v;
  m_sim_results->setPlainText(s.str().c_str());
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
void elly::qtmaindialog::set_init_n_mainland_sps(const int init_n_mainland_sps)
{
  ui->parameters->item( row_init_n_mainland_sps , 0)->setText(QString::number(init_n_mainland_sps));
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
  set_init_n_mainland_sps(p.get_init_n_mainland_sps());
  set_crown_age(p.get_crown_age());
  assert(get_parameters() == p);
}

void elly::qtmaindialog::setup_widgets() noexcept
{
  m_daic_input->setFont(QFont("Monospace"));
  m_daic_input->setMinimumHeight(400);
  m_daic_input->setReadOnly(true);
  m_plot_pop_sizes->setMinimumHeight(400);
  m_plot_rates->setMinimumHeight(400);
  m_sim_results->setFont(QFont("Monospace"));
  m_sim_results->setMinimumHeight(400);
  m_sim_results->setReadOnly(true);
}

void elly::qtmaindialog::on_start_next_clicked()
{
  set_rng_seed(get_rng_seed() + 1);
  on_start_clicked();
}

void elly::qtmaindialog::on_button_1_clicked()
{
  this->set_parameters(create_parameters_set1());
  assert(get_parameters() == create_parameters_set1());

}

void elly::qtmaindialog::on_button_2_clicked()
{
  this->set_parameters(create_parameters_set2());
  assert(get_parameters() == create_parameters_set2());

}

void elly::qtmaindialog::on_button_3_clicked()
{
  this->set_parameters(create_parameters_set3());
  assert(get_parameters() == create_parameters_set3());

}
