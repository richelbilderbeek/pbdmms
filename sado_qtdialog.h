#ifndef SADO_QTDIALOG_H
#define SADO_QTDIALOG_H

#include <array>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QDialog>
#include "sado_parameters.h"
#include "sado_fwd.h"
//#include "sado_results.h"
#pragma GCC diagnostic pop

struct QwtPlot;
struct QwtPlotCurve;

namespace Ui {
  class sado_qtdialog;
}

namespace sado {

class qtdialog : public QDialog //!OCLINT indeed to many methods, will fix this later
{
  Q_OBJECT //!OCLINT

public:
  explicit qtdialog(QWidget *parent = 0);
  qtdialog(const qtdialog&) = delete;
  qtdialog& operator=(const qtdialog&) = delete;
  ~qtdialog();

  parameters get_parameters() const;

  double get_b() const noexcept;
  double get_c() const noexcept;
  int get_end_time() const noexcept;
  erasure_method get_erase_method() const noexcept;
  double get_eta() const noexcept;
  double get_histbinp() const noexcept;
  double get_histbinq() const noexcept;
  double get_histbinx() const noexcept;
  next_generation_method get_next_gen_method() const noexcept;
  std::string get_output_filename() const noexcept;
  int get_output_freq() const noexcept;
  double get_p0() const noexcept;
  int get_pop_size() const noexcept;
  double get_q0() const noexcept;
  double get_sc() const noexcept;
  double get_se() const noexcept;
  int get_seed() const noexcept;
  double get_sk() const noexcept;
  double get_sm() const noexcept;
  double get_sq() const noexcept;
  double get_sv() const noexcept;
  bool get_use_initialization_bug() const noexcept;
  double get_x0() const noexcept;

  void set_parameters(const parameters& p) noexcept;

  void set_b(const double b) noexcept;
  void set_c(const double c) noexcept;
  void set_end_time(const int end_time) noexcept;
  void set_erase_method(const erasure_method em) noexcept;
  void set_eta(const double eta) noexcept;
  void set_histbinp(const double histbinp) noexcept;
  void set_histbinq(const double histbinq) noexcept;
  void set_histbinx(const double histbinx) noexcept;
  void set_next_gen_method(const next_generation_method ngm) noexcept;
  void set_output_filename(const std::string& output_filename) noexcept;
  void set_output_freq(const int output_freq) noexcept;
  void set_p0(const double p0) noexcept;
  void set_pop_size(const int pop_size) noexcept;
  void set_q0(const double q0) noexcept;
  void set_sc(const double sc) noexcept;
  void set_se(const double se) noexcept;
  void set_seed(const int seed) noexcept;
  void set_sk(const double sk) noexcept;
  void set_sm(const double sm) noexcept;
  void set_sq(const double sq) noexcept;
  void set_sv(const double sv) noexcept;
  void set_use_initialization_bug(const bool use_initialization_bug) noexcept;
  void set_x0(const double x0) noexcept;

private slots:
  void on_start_clicked();

private:
  Ui::sado_qtdialog *ui;

  void plot_timeseries(const results& r);

  QwtPlot * const m_plot;
  std::array<QwtPlotCurve *, 6> m_plot_lines;

};

std::array<QwtPlotCurve *, 6> create_initial_plot_lines() noexcept;

} //~namespace sado

#endif // SADO_QTDIALOG_H
