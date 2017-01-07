#ifndef ELLY_QTDIALOG_H
#define ELLY_QTDIALOG_H

#include <array>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#include "elly_fwd.h"
#include "elly_parameters.h"
#include "elly_measurements.h"
#include <QDialog>
#include "elly_events_rates_in_time.h"
#pragma GCC diagnostic pop

struct QwtPlot;
struct QwtPlotCurve;

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

  per_species_rate get_clad_is() const noexcept;
  per_species_rate get_clad_main() const noexcept;
  per_species_rate get_ana() const noexcept;
  per_species_rate get_ext_is() const noexcept;
  per_species_rate get_ext_main() const noexcept;
  per_species_rate get_mig_to_is() const noexcept;
  int get_carryingcap_is() const noexcept;
  int get_carryingcap_main() const noexcept;
  int get_rng_seed() const noexcept;
  int get_init_n_mainland() const noexcept;
  double get_crown_age() const noexcept;

  void set_parameters(const parameters &p) noexcept;

  void set_clad_is(const per_species_rate clado_is) noexcept;
  void set_clad_main(const per_species_rate clado_main) noexcept;
  void set_ana(const per_species_rate ana) noexcept;
  void set_ext_is(const per_species_rate ext_is) noexcept;
  void set_ext_main(const per_species_rate ext_main) noexcept;
  void set_mig_to_is(const per_species_rate mig_to_is) noexcept;
  void set_carryingcap_is(const int carryingcap_is);
  void set_carryingcap_main(const int carryingcap_main);
  void set_rng_seed(const int rng_seed) noexcept;
  void set_init_n_mainland(const int init_n_mainland);
  void set_crown_age(const double crown_age);


private slots:
  void on_start_clicked();

  void on_start_next_clicked();

private:
  Ui::elly_qtmaindialog *ui;

  void plot_event_rates(const measurements& v);
  void plot_pop_sizes(const measurements& v);

  QwtPlot * const m_plot_pop_sizes;
  QwtPlot * const m_plot_rates;
  std::array<QwtPlotCurve *, 6> m_curves_pop_sizes;
  std::array<QwtPlotCurve *, 10> m_curves_rates;
};

std::array<QwtPlotCurve *, 6> create_initial_curves_pop_sizes() noexcept;
std::array<QwtPlotCurve *, 10> create_initial_curves_rates() noexcept;

} //~namespace sado

#endif // ELLY_QTDIALOG_H
