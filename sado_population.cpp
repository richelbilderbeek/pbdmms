#include "sado_population.h"

#include "sado_attractiveness_vector.h"

#include <cassert>
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
