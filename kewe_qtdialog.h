#ifndef KEWE_QTDIALOG_H
#define KEWE_QTDIALOG_H

#include <array>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QDialog>
#include "kewe_simulation_parameters.h"
#include "kewe_results.h"
#include "kewe_ploidy.h"
#pragma GCC diagnostic pop

struct QwtPlot;
struct QwtPlotCurve;

namespace Ui {
class kewe_qtdialog;
}

namespace kewe {

class qtdialog : public QDialog
{
  Q_OBJECT //!OCLINT

public:
  explicit qtdialog(QWidget *parent = 0);
  qtdialog(const qtdialog&) = delete;
  qtdialog& operator=(const qtdialog&) = delete;
  ~qtdialog();

  ploidy get_ploidy() const noexcept;

  double get_end_time() const noexcept;

  simulation_parameters get_parameters() const noexcept;

  int get_population_size() const noexcept;

  void set_parameters(const simulation_parameters& p) noexcept;

private slots:
  void on_start_clicked();

private:
  Ui::kewe_qtdialog *ui;

  void plot_result_variables(const result_variables& r);

  QwtPlot * const m_plot;
  std::array<QwtPlotCurve *, 6> m_plot_lines;
};

std::array<QwtPlotCurve *, 6> create_initial_plot_lines() noexcept;

} //~namespace kewe

#endif // KEWE_QTDIALOG_H
