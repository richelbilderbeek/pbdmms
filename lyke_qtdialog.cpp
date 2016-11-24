#include "lyke_qtdialog.h"

#include <cassert>

#include<QFile>

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_point_data.h>
#include <qwt_text.h>
#include <qwt_legend.h>

#include "lyke_individual.h"
#include "lyke_simulation.h"
#include "pbd_helper.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "ui_lyke_qtdialog.h"
#pragma GCC diagnostic pop

std::vector<double> convert_to_vd(const std::vector<std::string>& v)
{
  std::vector<double> w;
  w.reserve(v.size());
  std::transform(
    std::begin(v),
    std::end(v),
    std::back_inserter(w),
    [](const auto& i) {
      return  std::stod(i);
    }
  );
  return w;
}

std::vector<double> convert_to_vd(const std::vector<int>& v)
{
  std::vector<double> w;
  w.reserve(v.size());
  std::transform(
    std::begin(v),
    std::end(v),
    std::back_inserter(w),
    [](const auto& i) { return static_cast<double>(i); }
  );
  return w;
}

lyke::qtdialog::qtdialog(QWidget *parent) :
  QDialog(parent),
  ui(new Ui::lyke_qtdialog),
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
    line->setPen(Qt::black, 2.0);
  }
  m_plot_lines[0]->setPen(QColor(255,0,0), 2.0);
  m_plot_lines[0]->setTitle(QwtText("The thing in lyke_defaultresults.csv"));

  {
    QwtLegend * const legend = new QwtLegend;
    legend->setFrameStyle(QFrame::Box|QFrame::Sunken);
    m_plot->insertLegend(legend, QwtPlot::RightLegend);
  }

  ui->histogram->setMinimumHeight(400);
}

lyke::qtdialog::~qtdialog()
{
  delete ui;
}

std::array<QwtPlotCurve *, 1> lyke::create_initial_plot_lines() noexcept
{
  std::array<QwtPlotCurve *, 1> v;
  for (auto& i: v) { i = new QwtPlotCurve; }
  return v;
}

std::vector<double> lyke::create_timeseries(const int max_t)
{
  std::vector<double> ts(max_t);
  std::iota(std::begin(ts), std::end(ts), 0.0);
  return ts;
}

int lyke::qtdialog::get_simulationruns() const { return ui->parameters->item(0,0)->text().toInt(); }
int    lyke::qtdialog::get_nGeneEco() const { return ui->parameters->item(1,0)->text().toInt(); }
double lyke::qtdialog::get_mu() const { return ui->parameters->item(2,0)->text().toDouble(); }
double lyke::qtdialog::get_sigmaMut() const { return ui->parameters->item(3,0)->text().toDouble(); }
int    lyke::qtdialog::get_popSize() const { return ui->parameters->item(4,0)->text().toInt(); }
double lyke::qtdialog::get_sigmac() const { return ui->parameters->item(5,0)->text().toDouble(); }
double lyke::qtdialog::get_sigmaK() const { return ui->parameters->item(6,0)->text().toDouble(); }
double lyke::qtdialog::get_alpha() const { return ui->parameters->item(7,0)->text().toDouble(); }
double lyke::qtdialog::get_beta() const { return ui->parameters->item(8,0)->text().toDouble(); }
int lyke::qtdialog::get_seed() const { return ui->parameters->item(9,0)->text().toInt(); }

void lyke::qtdialog::on_start_clicked()
{
  //Copied from main, BAD IDEA! Removed all std::couts
  {
    const int seed{get_seed()}; //Brilliant! A LOCAL variable!
    rnd::set_seed(seed);
    std::vector <Individual> population(popSize); //No use of those cool pointers anymore
    EcoTypeFilestream << "Generation" << ","
                      << "Ecotype" << ","
                      << "Individual" << "\n"; //output to csv.file
    HistogramFilestream << "Time,1,2,3,4,5,6,7,8,9,10,11,12,13,14" << '\n';



    for (int i = 0; i < static_cast<int>(simulationruns); ++i)  //number of generations
    {
      EcoTypeFilestream << 1 + i;
      iterate(population); // updates population
      doStatistics(population);
      doHistogram(population, i+1);
    }


    EcoTypeFilestream.close(); //closes excel file
    HistogramFilestream.close();
    DefaultresultsFiles.close();
  }

  plot_histogram();
  plot_defaultresults();
}



void lyke::qtdialog::plot_defaultresults()
{
  const std::vector<std::string> text = pbd::file_to_vector("lyke_defaultresults.csv");
  const std::vector<double> ys = convert_to_vd(text);
  const std::vector<double> xs = create_timeseries(ys.size());

  {
    assert(xs.size() == ys.size());
    QwtPointArrayData * const data = new QwtPointArrayData(&xs[0],&ys[0],xs.size());
    m_plot_lines[0]->setData(data);
  }
  m_plot->replot();
}

void lyke::qtdialog::plot_histogram()
{
  std::vector<std::vector<double>> histogram_in_time = read_histogram_in_time("Histogram.csv");
  ui->histogram->SetSurfaceGrey(histogram_in_time);
}

std::vector<std::vector<double>> lyke::read_histogram_in_time(const std::string& filename)
{
  std::vector<std::string> text = pbd::file_to_vector(filename);
  //Skip the first line, as this is the header
  text.erase(text.begin());

  std::vector<std::vector<double>> v(text.size());
  const int n_lines{static_cast<int>(text.size())};
  for (int i=0; i!=n_lines; ++i)
  {
    std::vector<std::string> words = pbd::seperate_string(text[i], ',');
    //Remove first word, as this is the generation number
    words.erase(words.begin());
    if (words.back() == "") words.pop_back(); //Lines end with a comma :-(
    v[i] = convert_to_vd(words);
  }
  return v;
}


void lyke::qtdialog::set_parameters()
{
  //Updates the globals (BAD IDEA!)
  g_parameters = lyke_parameters(
      get_simulationruns(),
      get_L(),
      get_nGeneEco(),
      get_mu(),
      get_sigmaMut(),
      get_popSize(),
      get_sigmac(),
      get_sigmaK(),
      get_alpha(),
      get_beta()
   );
}

