#include "sado_population.h"

#include "sado_attractiveness_vector.h"

#include <cassert>
#include <numeric>

#include <gsl/gsl>

#include "sado_simulation.h"

sado::population::population(const std::vector<indiv>& initial_population)
  : m_population{initial_population}
{

}


void sado::population::add_indiv(const indiv& i)
{
  m_population.push_back(i);
}

sado::population sado::create_test_population_0() noexcept
{
  return population();
}

sado::population sado::create_test_population_1() noexcept
{
  population p;
  p.add_indiv(indiv());
  return p;
}

sado::population sado::create_test_population_2() noexcept
{
  population p;
  p.add_indiv(indiv());
  p.add_indiv(indiv());

  // p[0] likes p[1]
  Ensures(get_attractivenesses(p,p[0].get_p(), p[0].get_x(), create_article_parameters())[1] > 0.05);
  // p[1] likes p[0]
  Ensures(get_attractivenesses(p,p[1].get_p(), p[1].get_x(), create_article_parameters())[0] > 0.05);
  return p;
}

sado::population sado::create_test_population_3() noexcept
{
  population p;
  p.add_indiv(
    indiv(create_null_id(), create_null_id(),
      -100.0, -100.0, -100.0, -100.0, -100.0, -100.0
    )
  );
  p.add_indiv(
    indiv(create_null_id(), create_null_id(),
       100.0,  100.0,  100.0,  100.0,  100.0,  100.0
    )
  );

  // p[0] dislikes p[1]
  Ensures(get_attractivenesses(p,p[0].get_p(), p[0].get_x(), create_article_parameters())[1] < 0.00001);
  // p[1] dislikes p[0]
  Ensures(get_attractivenesses(p,p[1].get_p(), p[1].get_x(), create_article_parameters())[0] < 0.00001);
  return p;
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
    rhs.get_population() == lhs.get_population()
  ;
}
bool sado::operator!=(const population& lhs, const population& rhs) noexcept
{
  return !(lhs == rhs);
}
