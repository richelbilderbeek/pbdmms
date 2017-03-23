#include "sado_population.h"

#include "sado_attractiveness_vector.h"

#include <cassert>
#include <cmath>
#include <numeric>

#include <gsl/gsl>

#include "sado_simulation.h"

sado::population::population(const std::vector<individual>& initial_population)
  : m_population{initial_population}
{

}


void sado::population::add_indiv(const individual& i)
{
  m_population.push_back(i);
}

double sado::calc_rhoxp(const population& pop)
{
  double ssxx = 0.0, ssxp = 0.0, sspp = 0.0, ssxq = 0.0, ssqq = 0.0, sspq = 0.0;
  const double avgx{get_mean_x(pop)};
  const double avgp{get_mean_p(pop)};
  const double avgq{get_mean_q(pop)};
  for (const auto &i : pop.get_population())
  {
    const double dxi{i.get_x() - avgx};
    const double dpi{i.get_p() - avgp};
    const double dqi{i.get_q() - avgq};
    ssxx += dxi * dxi;
    ssxp += dxi * dpi;
    ssxq += dxi * dqi;
    sspp += dpi * dpi;
    sspq += dpi * dqi;
    ssqq += dqi * dqi;
  }

  const double rhoxp{ssxp / std::sqrt(ssxx * sspp)};
  return rhoxp;
}

double sado::calc_rhoxq(const population& pop)
{
  double ssxx = 0.0, ssxp = 0.0, sspp = 0.0, ssxq = 0.0, ssqq = 0.0, sspq = 0.0;
  const double avgx{get_mean_x(pop)};
  const double avgp{get_mean_p(pop)};
  const double avgq{get_mean_q(pop)};
  for (const auto &i : pop.get_population())
  {
    const double dxi{i.get_x() - avgx};
    const double dpi{i.get_p() - avgp};
    const double dqi{i.get_q() - avgq};
    ssxx += dxi * dxi;
    ssxp += dxi * dpi;
    ssxq += dxi * dqi;
    sspp += dpi * dpi;
    sspq += dpi * dqi;
    ssqq += dqi * dqi;
  }

  const double rhoxq{ssxq / std::sqrt(ssxx * ssqq)};
  return rhoxq;
}

double sado::calc_rhopq(const population& pop)
{
  double ssxx = 0.0, ssxp = 0.0, sspp = 0.0, ssxq = 0.0, ssqq = 0.0, sspq = 0.0;
  const double avgx{get_mean_x(pop)};
  const double avgp{get_mean_p(pop)};
  const double avgq{get_mean_q(pop)};
  for (const auto &i : pop.get_population())
  {
    const double dxi{i.get_x() - avgx};
    const double dpi{i.get_p() - avgp};
    const double dqi{i.get_q() - avgq};
    ssxx += dxi * dxi;
    ssxp += dxi * dpi;
    ssxq += dxi * dqi;
    sspp += dpi * dpi;
    sspq += dpi * dqi;
    ssqq += dqi * dqi;
  }

  const double rhopq{sspq / std::sqrt(sspp * ssqq)};
  return rhopq;
}


double sado::calc_sp(const population& pop)
{
  assert(pop.size() > 1);
  double ssxx = 0.0;
  const double avgx{get_mean_p(pop)};
  for (const auto &i : pop.get_population())
  {
    const double dxi{i.get_p() - avgx};
    ssxx += dxi * dxi;
  }
  const double sx{
      std::sqrt(ssxx
    / (static_cast<double>(pop.size()) - 1.0))};
  return sx;
}

double sado::calc_sq(const population& pop)
{
  assert(pop.size() > 1);
  double ssxx = 0.0;
  const double avgx{get_mean_q(pop)};
  for (const auto &i : pop.get_population())
  {
    const double dxi{i.get_q() - avgx};
    ssxx += dxi * dxi;
  }
  const double sx{
      std::sqrt(ssxx
    / (static_cast<double>(pop.size()) - 1.0))};
  return sx;
}

double sado::calc_sx(const population& pop)
{
  assert(pop.size() > 1);
  double ssxx = 0.0;
  const double avgx{get_mean_x(pop)};
  for (const auto &i : pop.get_population())
  {
    const double dxi{i.get_x() - avgx};
    ssxx += dxi * dxi;
  }
  const double sx{
      std::sqrt(ssxx
    / (static_cast<double>(pop.size()) - 1.0))};
  return sx;
}


sado::population sado::create_test_population_0() noexcept
{
  return {};
}

sado::population sado::create_test_population_1() noexcept
{
  return { { individual() } };
}

sado::population sado::create_test_population_2() noexcept
{
  const individual a;
  const individual b;

  // p[0] likes p[1]
  //Ensures(get_attractivenesses(p,a.get_p(), a.get_x(), create_article_parameters())[1] > 0.05);
  // p[1] likes p[0]
  //Ensures(get_attractivenesses(p,b.get_p(), b.get_x(), create_article_parameters())[0] > 0.05);
  return { {a, b} };
}

sado::population sado::create_test_population_3() noexcept
{
  population p;
  const individual a(
    individual(create_null_id(), create_null_id(),
      -100.0, -100.0, -100.0, -100.0, -100.0, -100.0
    )
  );
  const individual b(
    individual(create_null_id(), create_null_id(),
       100.0,  100.0,  100.0,  100.0,  100.0,  100.0
    )
  );

  // p[0] dislikes p[1]
  //Ensures(get_attractivenesses(p,p[0].get_p(), p[0].get_x(), create_article_parameters())[1] < 0.00001);
  // p[1] dislikes p[0]
  //Ensures(get_attractivenesses(p,p[1].get_p(), p[1].get_x(), create_article_parameters())[0] < 0.00001);
  return { {a, b} };
}


void sado::population::downsize(const int smaller_size)
{
  assert(smaller_size <= size());
  m_population.resize(smaller_size);
}

const sado::individual& sado::population::operator[](const int i) const
{
  assert(i >= 0);
  assert(i < size());
  return m_population[i];
}

sado::individual& sado::population::operator[](const int i)
{
  assert(i >= 0);
  assert(i < size());
  return m_population[i];
}

double sado::get_mean_x(const population& p)
{
  assert(!p.empty());
  return std::accumulate(
             std::begin(p.get_population()),
             std::end(p.get_population()),
             0.0,
             [](const double init, const individual& i) {
               return init + i.get_x();
             }) /
         static_cast<double>(p.size());
}

double sado::get_mean_p(const population& p)
{
  assert(!p.empty());
  return std::accumulate(
             std::begin(p.get_population()),
             std::end(p.get_population()),
             0.0,
             [](const double init, const individual& i) {
               return init + i.get_p();
             }) /
         static_cast<double>(p.size());
}

double sado::get_mean_q(const population& p)
{
  assert(!p.empty());
  return std::accumulate(
             std::begin(p.get_population()),
             std::end(p.get_population()),
             0.0,
             [](const double init, const individual& i) {
               return init + i.get_q();
             }) /
         static_cast<double>(p.size());
}

bool sado::operator==(const population& lhs, const population& rhs) noexcept
{
  return
    rhs.get_population() == lhs.get_population()
  ;
}
bool sado::operator!=(const population& lhs, const population& rhs) noexcept
{
  return !(lhs == rhs);
}
