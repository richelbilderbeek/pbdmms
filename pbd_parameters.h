#ifndef PBD_PARAMETERS_H
#define PBD_PARAMETERS_H

#include <iosfwd>
#include <string>

namespace pbd {

class parameters
{
  public:
  parameters(
    const double birth_good,
    const double birth_incipient,
    const double completion,
    const double death_good,
    const double death_incipient,
    const double time,
    const int seed,
    const std::string& pbd_nltt_filename = "jobo_pbd_nltt.csv"
  );

  const std::string& get_nltt_plot_filename() const noexcept { return m_pbd_nltt_filename;}
  double get_birth_good() const noexcept { return m_birth_good;}
  double get_birth_incipient() const noexcept {return m_birth_incipient;}
  double get_completion() const noexcept {return m_completion;}
  double get_death_good() const noexcept {return m_death_good;}
  double get_death_incipient() const noexcept {return m_death_incipient;}
  double get_time() const noexcept {return m_time;}
  int get_seed() const noexcept {return m_seed;}

private:
  double m_birth_good;
  double m_birth_incipient;
  double m_completion;
  double m_death_good;
  double m_death_incipient;
  double m_time;
  int m_seed;
  std::string m_pbd_nltt_filename;

  friend std::ostream& operator<<(std::ostream& os, const parameters& p) noexcept;
  friend std::istream& operator>>(std::istream& os, parameters& p);
};

parameters load_parameters_from_file(const std::string& filename);
void save(const parameters& p, const std::string& filename);
parameters create_parameters_set1() noexcept;
parameters create_parameters_set2() noexcept;
parameters create_parameters_set3() noexcept;

bool operator==(const parameters& lhs, const parameters& rhs) noexcept;
bool operator!=(const parameters& lhs, const parameters& rhs) noexcept;
std::ostream& operator<<(std::ostream& os, const parameters& p) noexcept;
std::istream& operator>>(std::istream& os, parameters& p);

} //~namespace pbd

#endif // PBD_PARAMETERS_H
