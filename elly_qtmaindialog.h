#ifndef ELLY_QTDIALOG_H
#define ELLY_QTDIALOG_H

#include <array>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "elly_fwd.h"
#include "elly_parameters.h"
#include "elly_measurements.h"
#include "elly_carrying_capacity.h"
#include "elly_per_species_rates.h"
#include "daic_input.h"
#include <QDialog>
#pragma GCC diagnostic pop

struct QwtPlot;
struct QwtPlotCurve;
struct QPlainTextEdit;

namespace Ui
{
  class elly_qtmaindialog;
}

namespace elly
{

class qtmaindialog : public QDialog //!OCLINT indeed to many methods, will fix this later
{
  Q_OBJECT //!OCLINT

  public : explicit qtmaindialog(QWidget *parent = 0);
  qtmaindialog(const qtmaindialog &) = delete;
  qtmaindialog &operator=(const qtmaindialog &) = delete;
  ~qtmaindialog();

  parameters get_parameters() const;
  per_species_rates get_rates() const;

  per_species_rate get_clad_is() const;
  per_species_rate get_clad_main() const;
  per_species_rate get_ana() const;
  per_species_rate get_ext_is() const;
  per_species_rate get_ext_main() const;
  per_species_rate get_mig_to_is() const;
  carrying_capacity get_carryingcap_is() const;
  carrying_capacity get_carryingcap_main() const;
  int get_rng_seed() const;
  int get_init_n_main_cls() const; //cls: clades
  int get_init_n_main_sps() const; //sps: species
  double get_crown_age() const;


  void set_parameters(const parameters& p) noexcept;
  void set_rates(const per_species_rates& r) noexcept;

  void set_clad_is(const per_species_rate clado_is) noexcept;
  void set_clad_main(const per_species_rate clado_main) noexcept;
  void set_ana(const per_species_rate ana) noexcept;
  void set_ext_is(const per_species_rate ext_is) noexcept;
  void set_ext_main(const per_species_rate ext_main) noexcept;
  void set_mig_to_is(const per_species_rate mig_to_is) noexcept;
  void set_carryingcap_is(const carrying_capacity carryingcap_is);
  void set_carryingcap_main(const carrying_capacity carryingcap_main);
  void set_rng_seed(const int rng_seed) noexcept;
  void set_init_n_main_cls(const int init_n_mainland_cls); //cls: clades
  void set_init_n_main_sps(const int init_n_mainland_sps); //sps: species
  void set_crown_age(const double crown_age);

public slots:

  void on_start_clicked();

private slots:

  void on_start_next_clicked();

  void on_button_1_clicked();
  void on_button_2_clicked();
  void on_button_3_clicked();

  void on_run_daisie_clicked();

  void on_button_load_clicked();

private:
  Ui::elly_qtmaindialog *ui;

  void add_plotting_peripherals() noexcept;
  void add_widgets_to_ui() noexcept;
  void attach_curves_to_plots() noexcept;
  void display_parameters(const parameters& p);

  void plot_daic_input(const results& v);
  void plot_daic_inputs(const experiment& e);
  void plot_daic_inputs(const daic::input& ideal, const daic::input& reality);
  void plot_daic_outputs(const experiment& e);
  void plot_event_rates(const measurements& v);
  void plot_pop_sizes(const measurements& v);
  void plot_sim_results(const results& v);
  void setup_widgets() noexcept;

  std::array<QwtPlotCurve *, 6> m_curves_pop_sizes;
  std::array<QwtPlotCurve *, 10> m_curves_rates;
  QPlainTextEdit * const m_daic_inputs;
  QPlainTextEdit * const m_daic_outputs;
  QPlainTextEdit * const m_parameters;
  QwtPlot * const m_plot_pop_sizes;
  QwtPlot * const m_plot_rates;
  QPlainTextEdit * const m_sim_results;
};

std::array<QwtPlotCurve *, 6> create_initial_curves_pop_sizes() noexcept;
std::array<QwtPlotCurve *, 10> create_initial_curves_rates() noexcept;

} //~namespace sado

#endif // ELLY_QTDIALOG_H
