#ifndef PBD_QTMAINDIALOG_H
#define PBD_QTMAINDIALOG_H

#include <QDialog>
#include "pbd_l_table.h"
#include "pbd_nltt.h"
#include "pbd_parameters.h"

namespace Ui {
  class pbd_qtmaindialog;
}

struct QwtPlot;
struct QwtPlotCurve;

namespace pbd {

class qtmaindialog : public QDialog
{
  Q_OBJECT //!OCLINT

public:
  explicit qtmaindialog(QWidget *parent = 0);
  qtmaindialog(const qtmaindialog&) = delete;
  qtmaindialog& operator=(const qtmaindialog&) = delete;
  ~qtmaindialog();

  parameters get_parameters() const noexcept;

private slots:

  void on_start_clicked();

private:
  Ui::pbd_qtmaindialog *ui;
  QwtPlot * const m_nltt_plot_igtree_extinct;
  QwtPlot * const m_nltt_plot_recontree;
  QwtPlotCurve * const m_nltt_plot_igtree_extinct_line;
  QwtPlotCurve * const m_nltt_plot_recontree_line;

  void display_l_table(const l_table& t);
  void display_nltt_igtree_extinct(const nltt& points);
  void display_nltt_recon(const nltt& points);
  void display_png(const std::string& png_filename);
  void resize_table(const int n_rows);
};

} //~namespace pbd

#endif // PBD_QTMAINDIALOG_H
