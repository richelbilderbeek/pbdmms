#ifndef JOBO_QTMAINDIALOG_H
#define JOBO_QTMAINDIALOG_H

#include "jobo_parameters.h"
#include "pbd_ltt.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QDialog>
#pragma GCC diagnostic pop

namespace Ui {
  class jobo_qtmaindialog;
}

struct QwtPlot;
struct QwtPlotCurve;

namespace jobo {

class qtmaindialog : public QDialog
{
  Q_OBJECT //!OCLINT

public:
  explicit qtmaindialog(QWidget *parent = 0);
  qtmaindialog(const qtmaindialog&) = delete;
  qtmaindialog& operator=(const qtmaindialog&) = delete;
  ~qtmaindialog();

  ///Will throw if current values are invalid
  parameters get_parameters() const;

private slots:
  void on_button_clicked();

private:
  Ui::jobo_qtmaindialog *ui;

  QwtPlot * const m_ltt_plot;
  QwtPlotCurve * m_ltt_inviables;
  QwtPlotCurve * m_ltt_viables;

  void show_results(const parameters& p);
  void display_ltts(const pbd::ltt& viables, const pbd::ltt& inviables);
};

} //~namespace jobo

#endif // JOBO_QTMAINDIALOG_H
