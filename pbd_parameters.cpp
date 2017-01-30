#include "pbd_parameters.h"
//#include "jobo_parameters.h"
//#include "jobo_individuals.h"
//#include "jobo_individual.h"
#include <iostream>
#include <stdexcept>
#include <vector>
#include <cassert>
#include <fstream>
#include <algorithm>
#include <set>
#include <cstdio>
#include <cctype>
#include <string>
#include <random>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include "pbd_helper.h"

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

pbd::parameters pbd::create_profile_parameters_set() noexcept
{
  const double birth_good {0.5};
  const double birth_incipient {0.5};
  const double completion {0.1};
  const double death_good {0.1};
  const double death_incipient {0.1};
  const double time {10.0};
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

pbd::parameters pbd::create_parameters_set1() noexcept
{
  const double birth_good {0.5};
  const double birth_incipient {0.5};
  const double completion {0.1};
  const double death_good {0.1};
  const double death_incipient {0.1};
  const double time {10.0};
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
  const double time {100.0};
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
    << "birth_good: " << p.m_birth_good << ' '
    << "birth_incipient: " << p.m_birth_incipient << ' '
    << "completion: " << p.m_completion << ' '
    << "death_good: " << p.m_death_good << ' '
    << "death_incipient: " << p.m_death_incipient << ' '
    << "time: " << p.m_time << ' '
    << "seed: " << p.m_seed << ' '
    ;
   return os;
}

pbd::parameters pbd::load_parameters(const std::string& filename)
{
  //Check if there is a parameter file
  if (!is_regular_file(filename))
  {
    throw std::invalid_argument("parameter file cannot be found");
  }
  std::ifstream f(filename);
  parameters p = create_parameters_set1(); //Just any, will be overwritten anyways
  f >> p;
  return p;
}

std::istream& pbd::operator>>(std::istream& is, parameters& p)
{
  double birth_good{0.0};
  double birth_incipient{0.0};
  double completion {0.0};
  double death_good {0.0};
  double death_incipient {0.0};
  double time {0.0};
  int seed {0};
  std::string s;

  is >> s >> birth_good;
  assert(s == "birth_good:");
  is >> s >> birth_incipient;
  assert(s == "birth_incipient:");
  is >> s >> completion;
  assert(s == "completion:");
  is >> s >> death_good;
  assert(s == "death_good:");
  is >> s >> death_incipient;
  assert(s == "death_incipient:");
  is >> s >> time;
  assert(s == "time:");
  is >> s >> seed;
  assert(s == "seed:");

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

