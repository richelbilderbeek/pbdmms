#include "jobo_qtmaindialog.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include <chrono>
#include <sstream>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_point_data.h>
#include <qwt_text.h>


#include "file_to_vector.h"
#include "jobo_jkr_adapters.h"
#include "jkr_experiment.h"
#include "is_regular_file.h"
#include "jobo_parameters.h"
#include "jobo_results.h"
#include "jobo_simulation.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QDebug>
#include "ui_jobo_qtmaindialog.h"
#pragma GCC diagnostic pop

jobo::qtmaindialog::qtmaindialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::jobo_qtmaindialog),
  m_ltt_plot{new QwtPlot(QwtText("LTT"), this)},
  m_ltt_inviables{new QwtPlotCurve},
  m_ltt_viables{new QwtPlotCurve}
{
  ui->setupUi(this);

  assert(ui->scroll_area_contents->layout());
  ui->scroll_area_contents->layout()->addWidget(m_ltt_plot);

  m_ltt_plot->setMinimumHeight(400);
  m_ltt_viables->attach(m_ltt_plot);
  m_ltt_viables->setStyle(QwtPlotCurve::Steps);
  m_ltt_viables->setPen(Qt::green, 2.0);
  m_ltt_inviables->attach(m_ltt_plot);
  m_ltt_inviables->setStyle(QwtPlotCurve::Steps);
  m_ltt_inviables->setPen(Qt::black, 2.0);

 // on_button_clicked();
}

jobo::qtmaindialog::~qtmaindialog()
{
  delete ui;
}

void jobo::qtmaindialog::display_ltts(const pbd::ltt& viables, const pbd::ltt& inviables)
{
  if (viables.empty()) return;
  assert(inviables.size() == viables.size());
  std::vector<double> xs;
  std::vector<double> ys_viables;
  for (const auto p: viables.get())
  {
    xs.push_back(p.first);
    ys_viables.push_back(static_cast<double>(p.second));
  }
  std::vector<double> ys_inviables;
  for (const auto p: inviables.get())
  {
    ys_inviables.push_back(static_cast<double>(p.second));
  }
  assert(!xs.empty());
  assert(ys_viables.size() == ys_inviables.size());
  assert(!ys_viables.empty());
  QwtPointArrayData * const data_viables = new QwtPointArrayData(&xs[0],&ys_viables[0],xs.size());
  QwtPointArrayData * const data_inviables = new QwtPointArrayData(&xs[0],&ys_inviables[0],xs.size());
  m_ltt_viables->setData(data_viables);
  m_ltt_inviables->setData(data_inviables);
  m_ltt_plot->replot();
}

jobo::parameters jobo::qtmaindialog::get_parameters() const
{
  const int population_size{
    std::stoi(ui->parameters->item(0,0)->text().toStdString())
  };
  const int seed{
    std::stoi(ui->parameters->item(1,0)->text().toStdString())
  };
  const double mutation_rate{
    std::stod(ui->parameters->item(2,0)->text().toStdString())
  };
  const int n_generations{
    std::stoi(ui->parameters->item(3,0)->text().toStdString())
  };
  const int loci{
    std::stoi(ui->parameters->item(4,0)->text().toStdString())
  };
  return parameters(
    population_size,
    seed,
    mutation_rate,
    n_generations,
    loci
  );
}

void jobo::qtmaindialog::on_button_clicked()
{
  const auto p = get_parameters();
  simulation s(p);
  const int n_generations = get_n_generations(this->get_parameters());

  using my_clock = std::chrono::high_resolution_clock;
  const auto start_time = my_clock::now();
  ui->progress_bar->setMaximum(n_generations);

  for (int t=0; t!=n_generations; ++t)
  {
    s.do_timestep();
    ui->progress_bar->setValue(t);
  }

  ui->progress_bar->setValue(n_generations);
  {
    const auto end_time = my_clock::now();
    const auto diff = end_time - start_time;
    std::stringstream t;
    t << "Simulation lasted "
      << std::chrono::duration_cast<std::chrono::seconds>(diff).count()
      << " seconds";
    ui->label_sim_runtime->setText(t.str().c_str());

  }

  save_ltt_plot(jobo::get_results(s), get_ltt_plot_filename(s.get_parameters()));

  pbd::ltt ltt_viables;
  {
    const std::vector<int> n_lineages{s.get_results().get_ltt_viables()};
    const int sz{static_cast<int>(n_lineages.size())};
    for (int i=0; i!=sz; ++i)
    {
      const double t{static_cast<double>(i)};
      const int n{n_lineages[i]};
      ltt_viables.add_timepoint(t, n);
    }
  }
  pbd::ltt ltt_inviables;
  {
    const std::vector<int> n_lineages{s.get_results().get_ltt_inviables()};
    const int sz{static_cast<int>(n_lineages.size())};
    for (int i=0; i!=sz; ++i)
    {
      const double t{static_cast<double>(i)};
      const int n{n_lineages[i]};
      ltt_inviables.add_timepoint(t, n);
    }
  }

  display_ltts(ltt_viables, ltt_inviables);
}

