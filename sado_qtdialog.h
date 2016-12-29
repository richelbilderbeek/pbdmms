#ifndef SADO_QTDIALOG_H
#define SADO_QTDIALOG_H

#include <array>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include <QDialog>
#include "sado_parameters.h"
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
  std::string& get_output_filename() const noexcept;
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

  void set_eco_res_distr_width(const double eco_res_distr_width);
  void set_eco_res_util_width(const double eco_res_util_width);
  void set_initial_eco_trait(const double initial_eco_trait) noexcept;
  void set_initial_fem_pref(const double initial_fem_pref) noexcept;
  void set_initial_male_trait(const double initial_male_trait) noexcept;
  void set_mate_spec_eco(const double mate_spec_eco);
  void set_mate_spec_mate(const double mate_spec_mate);
  void set_mut_distr_width(const double mut_distr_width);

  void set_parameters(const parameters& p) noexcept;

  ///Viability selection on male mating type strength
  void set_viab_male_mate_str(const double viab_male_mate_str);


private slots:
  void on_start_clicked();

  void on_set_branching_clicked();

  void on_show_branching_clicked();

private:
  Ui::sado_qtdialog *ui;

  //void plot_result_variables(const result_variables& r);

  QwtPlot * const m_plot;
  std::array<QwtPlotCurve *, 6> m_plot_lines;
};

std::array<QwtPlotCurve *, 6> create_initial_plot_lines() noexcept;

} //~namespace sado

#endif // SADO_QTDIALOG_H
