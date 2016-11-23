#ifndef lyke_QTDIALOG_H
#define lyke_QTDIALOG_H

#include <array>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QDialog>
#include "lyke_parameters.h"
#pragma GCC diagnostic pop

struct QwtPlot;
struct QwtPlotCurve;

namespace Ui {
class lyke_qtdialog;
}

namespace lyke {

class qtdialog : public QDialog
{
  Q_OBJECT //!OCLINT

public:
  explicit qtdialog(QWidget *parent = 0);
  qtdialog(const qtdialog&) = delete;
  qtdialog& operator=(const qtdialog&) = delete;
  ~qtdialog();

  //All parameters
  int get_simulationruns() const;
  int    get_nGeneEco() const;
  double get_mu() const;
  double get_sigmaMut() const;
  int    get_popSize() const;
  double get_sigmac() const;
  double get_sigmaK() const;
  double get_alpha() const;
  double get_beta() const;
  int get_seed() const;

private slots:
  void on_start_clicked();

private:
  Ui::lyke_qtdialog *ui;

  void set_parameters();

  ///Plots the file histogram.csv
  void plot_histogram();

  ///Plots the file lyke_defaultresults.csv
  void plot_defaultresults();

  QwtPlot * const m_plot;
  std::array<QwtPlotCurve *, 1> m_plot_lines;
};

std::array<QwtPlotCurve *, 1> create_initial_plot_lines() noexcept;

///Creates a timeseries like { 0.0, 1,0, 2.0, ..., max_t-1.0 }
std::vector<double> create_timeseries(const int max_t);

///Assumes the file exists
std::vector<std::vector<double>> read_histogram_in_time(const std::string& filename);


} //~namespace lyke

#endif // lyke_QTDIALOG_H
