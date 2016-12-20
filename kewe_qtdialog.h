#ifndef KEWE_QTDIALOG_H
#define KEWE_QTDIALOG_H

#include <array>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QDialog>
#include "kewe_parameters.h"
#include "kewe_results.h"
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

  parameters get_parameters() const;

private slots:
  void on_start_clicked();

  void on_checkBox_clicked();

  void on_checkBox_2_clicked();

private:
  Ui::kewe_qtdialog *ui;

  void plot_result_variables(const result_variables& r);

  QwtPlot * const m_plot;
  std::array<QwtPlotCurve *, 6> m_plot_lines;
};

std::array<QwtPlotCurve *, 6> create_initial_plot_lines() noexcept;

} //~namespace kewe

#endif // KEWE_QTDIALOG_H
