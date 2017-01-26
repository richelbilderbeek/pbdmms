#include "pbd_parameters.h"

#include <cassert>
#include <fstream>
#include <iostream>

pbd::parameters::parameters(
  const double birth_good,
  const double birth_incipient,
  const double completion,
  const double death_good,
  const double death_incipient,
  const double time,
  const int seed,
  const std::string& pbd_nltt_filename
)
  : m_birth_good{birth_good},
    m_birth_incipient{birth_incipient},
    m_completion{completion},
    m_death_good{death_good},
    m_death_incipient{death_incipient},
    m_time{time},
    m_seed{seed},
    m_pbd_nltt_filename{pbd_nltt_filename}
{
  assert(m_birth_good >= 0.0);
  assert(m_birth_incipient >= 0.0);
  assert(m_completion >= 0.0);
  assert(m_death_good >= 0.0);
  assert(m_death_incipient >= 0.0);
  assert(m_time >= 0.0);
}

pbd::parameters pbd::create_parameters_set1() noexcept
{
  const double birth_good {0.5};
  const double birth_incipient {0.5};
  const double completion {0.1};
  const double death_good {0.1};
  const double death_incipient {0.1};
  const double time {100};
  const int seed {42};
  return parameters (
    birth_good,
    birth_incipient,
    completion,
    death_good,
    death_incipient,
    time,
    seed
  );
}

pbd::parameters pbd::create_parameters_set2() noexcept
{
  const double birth_good {0.5};
  const double birth_incipient {0.5};
  const double completion {0.01};
  const double death_good {0.2};
  const double death_incipient {0.2};
  const double time {100};
  const int seed {42};
  return parameters (
    birth_good,
    birth_incipient,
    completion,
    death_good,
    death_incipient,
    time,
    seed
  );
}

pbd::parameters pbd::create_parameters_set3() noexcept
{
  const double birth_good {0.5};
  const double birth_incipient {0.5};
  const double completion {0.01};
  const double death_good {0.4};
  const double death_incipient {0.4};
  const double time {100};
  const int seed {42};
  return parameters (
    birth_good,
    birth_incipient,
    completion,
    death_good,
    death_incipient,
    time,
    seed
  );
}

void pbd::save(const parameters& p, const std::string& filename)
{
  std::ofstream f(filename);
  f << p;
}

pbd::parameters pbd::load_parameters_from_file(const std::string& filename)
{
  std::ifstream f(filename);
  parameters p = create_parameters_set1();
  f >> p;
  return p;
}

bool pbd::operator==(const parameters& lhs, const parameters& rhs) noexcept
{
  return lhs.get_birth_good() == rhs.get_birth_good()
      && lhs.get_birth_incipient() == rhs.get_birth_incipient()
      && lhs.get_completion() == rhs.get_completion()
      && lhs.get_death_good() == rhs.get_death_good()
      && lhs.get_death_incipient() == rhs.get_death_incipient()
      && lhs.get_time() == rhs.get_time()
      && lhs.get_seed() == rhs.get_seed()
  ;
}

bool pbd::operator!=(const parameters& lhs, const parameters& rhs) noexcept
{
  return !(lhs == rhs);
}


std::ostream& pbd::operator<<(std::ostream& os, const parameters& p) noexcept
{
  os
    << p.m_birth_good << ' '
    << p.m_birth_incipient << ' '
    << p.m_completion << ' '
    << p.m_death_good << ' '
    << p.m_death_incipient << ' '
    << p.m_time << ' '
    << p.m_seed << ' '
    ;
   return os;
}

/*
std::istream& pbd::operator>>(std::istream& is, parameters& p)
{
  //Must be in this order
  p.m_birth_good = p.get_birth_good();
  p.m_birth_incipient = p.get_birth_incipient();
  p.m_completion = p.get_completion();
  p.m_death_good = p.get_death_good();
  p.m_death_incipient = p.get_death_incipient();
  p.m_time = p.get_time();
  p.m_seed = p.seed();
  return is;
}
*/

std::istream& pbd::operator>>(std::istream& is, parameters& p)
{
  double birth_good{0.0};
  double birth_incipient{0.0};
  double completion {0.0};
  double death_good {0.0};
  double death_incipient {0.0};
  double time {0.0};
  int seed {0};

  is
    >> birth_good
    >> birth_incipient
    >> completion
    >> death_good
    >> death_incipient
    >> time
    >> seed
    ;
  p = parameters(
    birth_good,
    birth_incipient,
    completion,
    death_good,
    death_incipient,
    time,
    seed
  );
  return is;
}

