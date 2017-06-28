#include "sado_individual.h"

#include <algorithm>
#include <cassert>
#include <iostream>

#include "sado_helper.h"
#include "sado_parameters.h"
#include "sado_random.h"

using phyg::create_new_id;
using phyg::create_null_id;

sado::individual::individual(
    const id mother_id,
    const id father_id,
    const double p,
    const double q,
    const double x,
    const double p_gen,
    const double q_gen,
    const double x_gen)
    : m_id{create_new_id()}, m_id_mother{mother_id}, m_id_father{father_id},
      m_p{p}, m_q{q}, m_x{x}, m_p_gen{p_gen}, m_q_gen{q_gen}, m_x_gen{x_gen}
{
  assert(m_id != create_null_id());
}

bool sado::all_have_unique_ids(const std::vector<individual>& v)
{
  std::vector<id> ids;
  ids.reserve(v.size());
  std::transform(
    std::begin(v), std::end(v),
    std::back_inserter(ids),
    [](const auto& i){ return i.get_id(); }
  );
  assert(ids.size() == v.size());
  std::sort(std::begin(ids), std::end(ids));
  return std::unique(std::begin(ids), std::end(ids)) == std::end(ids);
}

sado::individual
sado::create_offspring(const individual& m, const individual& f, const parameters& p)
{
  const double sv{p.get_sv()};
  // Note that genotype == phenotype
  // Do not change order for Golden Standard
  const double x_gen{(Uniform() < 0.5 ? m.m_x_gen : f.m_x_gen) +
                     Normal(0.0, sv)};
  const double p_gen{(Uniform() < 0.5 ? m.m_p_gen : f.m_p_gen) +
                     Normal(0.0, sv)};
  const double q_gen{(Uniform() < 0.5 ? m.m_q_gen : f.m_q_gen) +
                     Normal(0.0, sv)};
  return individual(m.get_id(), f.get_id(), p_gen, q_gen, x_gen, p_gen, q_gen, x_gen);
}

sado::individual sado::create_init_with_bug(
    const double this_x0,
    const double this_p0,
    const double this_q0,
    const parameters& p)
{
  const double sv{p.get_sv()};
  // This is a bug (see https://github.com/richelbilderbeek/pbdmms/issues/163 ):
  // at initialization, the phenotype and genotype is unrelated
  // Do not reorder, otherwise the Golden Standard will be different
  const double x_gen{this_x0 + Normal(0.0, sv)};
  const double p_gen{this_p0 + Normal(0.0, sv)};
  const double q_gen{this_q0 + Normal(0.0, sv)};
  const double x{this_x0 + Normal(0.0, sv)};
  const double ph{this_p0 + Normal(0.0, sv)};
  const double q{this_q0 + Normal(0.0, sv)};
  return individual(
    create_null_id(),
    create_null_id(),
    ph, q, x, p_gen, q_gen, x_gen);
}

std::ostream &sado::operator<<(std::ostream &os, const individual i) noexcept
{
  os << i.m_x << " " << i.m_p << " " << i.m_q << '\n';
  os << i.m_x_gen << " ";
  os << '\n';
  os << i.m_p_gen << " ";
  os << '\n';
  os << i.m_q_gen << " ";
  os << '\n';
  return os;
}

bool sado::operator==(const individual& lhs, const individual& rhs) noexcept
{
  return lhs.get_x() == rhs.get_x() && lhs.get_p() == rhs.get_p() &&
         lhs.get_q() == rhs.get_q() && lhs.get_id() == rhs.get_id();
}

bool sado::operator!=(const individual& lhs, const individual& rhs) noexcept
{
  return !(lhs == rhs);
}

bool sado::operator<(const individual& lhs, const individual& rhs) noexcept
{
  return lhs.get_id() < rhs.get_id();
}
