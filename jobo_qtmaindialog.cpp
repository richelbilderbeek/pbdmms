#include "jobo_qtmaindialog.h"

#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>

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
  m_ltt_plot_line{new QwtPlotCurve}
{
  ui->setupUi(this);

  assert(ui->scroll_area_contents->layout());
  ui->scroll_area_contents->layout()->addWidget(m_ltt_plot);

  m_ltt_plot->setMinimumHeight(400);
  m_ltt_plot_line->attach(m_ltt_plot);
  m_ltt_plot_line->setStyle(QwtPlotCurve::Steps);
  m_ltt_plot_line->setPen(Qt::black, 2.0);

  on_button_clicked();
}

jobo::qtmaindialog::~qtmaindialog()
{
  delete ui;
}

void jobo::qtmaindialog::display_ltt(const pbd::ltt& l)
{
  if (l.empty()) return;
  std::vector<double> xs;
  std::vector<double> ys;
  for (const auto p: l.get())
  {
    xs.push_back(p.first);
    ys.push_back(static_cast<double>(p.second));
  }
  assert(!xs.empty());
  assert(!ys.empty());
  QwtPointArrayData * const data = new QwtPointArrayData(&xs[0],&ys[0],xs.size());
  m_ltt_plot_line->setData(data);
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
  try
  {
    jkr::do_experiment
    <
      jobo::parameters,
      jobo::simulation,
      jobo::results
    >(p);
    const auto my_ltt = pbd::load_ltt_from_csv_one_liner(get_ltt_plot_filename(p));
    std::clog << my_ltt << '\n';
    display_ltt(my_ltt);
  }
  catch (std::exception& e)
  {
    ui->result->setText(e.what());
  }
}
