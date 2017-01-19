#include "sado_population.h"

#include <cassert>
#include <numeric>

sado::population::population(const std::vector<indiv>& initial_population)
  : m_pedigree{},
    m_population{initial_population}
{

}


void sado::population::downsize(const int smaller_size)
{
  assert(smaller_size <= size());
  m_population.resize(smaller_size);
}

const sado::indiv& sado::population::operator[](const int i) const
{
  assert(i >= 0);
  assert(i < size());
  return m_population[i];
}

sado::indiv& sado::population::operator[](const int i)
{
  assert(i >= 0);
  assert(i < size());
  return m_population[i];
}

double sado::get_mean_x(const population &p)
{
  assert(!p.empty());
  return std::accumulate(
             std::begin(p.get_population()),
             std::end(p.get_population()),
             0.0,
             [](const double init, const indiv &i) {
               return init + i.get_x();
             }) /
         static_cast<double>(p.size());
}

double sado::get_mean_p(const population &p)
{
  assert(!p.empty());
  return std::accumulate(
             std::begin(p.get_population()),
             std::end(p.get_population()),
             0.0,
             [](const double init, const indiv &i) {
               return init + i.get_p();
             }) /
         static_cast<double>(p.size());
}

double sado::get_mean_q(const population &p)
{
  assert(!p.empty());
  return std::accumulate(
             std::begin(p.get_population()),
             std::end(p.get_population()),
             0.0,
             [](const double init, const indiv &i) {
               return init + i.get_q();
             }) /
         static_cast<double>(p.size());
}

bool sado::operator==(const population& lhs, const population& rhs) noexcept
{
  return
        rhs.get_pedigree()   == lhs.get_pedigree()
    &&  rhs.get_population() == lhs.get_population()
  ;
}
bool sado::operator!=(const population& lhs, const population& rhs) noexcept
{
  return !(lhs == rhs);
}
