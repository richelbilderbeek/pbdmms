#ifndef RIBI_QTMAINDIALOG_H
#define RIBI_QTMAINDIALOG_H

#include "ribi_parameters.h"
#include "pbd_ltt.h"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QDialog>
#pragma GCC diagnostic pop

namespace Ui {
  class ribi_qtmaindialog;
}

struct QwtPlot;
struct QwtPlotCurve;

namespace ribi{

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

  void set_parameters(const parameters& p) const;

private slots:
  void on_button_clicked();
  void on_load_clicked();

private:
  Ui::ribi_qtmaindialog *ui;

  QwtPlot * const m_ltt_plot;
  QwtPlotCurve * m_ltt_plot_line;

  void delete_old_files(const parameters& p);
  void show_results(const parameters& p);
  void display_ltt(const pbd::ltt& l);
};

} //~namespace ribi

#endif // RIBI_QTMAINDIALOG_H
