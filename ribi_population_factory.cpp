#include "distancer_population_factory.h"

population population_factory::create(
  int n_a, const individual& i_a,
  int n_b, const individual& i_b,
  int n_c, const individual& i_c
) const noexcept
{
  population p;
  const population a(n_a, i_a);
  assert(n_a == static_cast<int>(a.size()));
  const population b(n_b, i_b);
  assert(n_b == static_cast<int>(b.size()));
  const population c(n_c, i_c);
  assert(n_c == static_cast<int>(c.size()));
  std::copy(std::begin(a), std::end(a), std::back_inserter(p));
  std::copy(std::begin(b), std::end(b), std::back_inserter(p));
  std::copy(std::begin(c), std::end(c), std::back_inserter(p));
  assert(n_a + n_b + n_c == static_cast<int>(p.size()));
  return p;
}

population population_factory::create(
  int n_a, const individual& i_a,
  int n_b, const individual& i_b,
  int n_c, const individual& i_c,
  int n_d, const individual& i_d,
  int n_e, const individual& i_e
) const noexcept
{
  population p;
  const population a(n_a, i_a);
  assert(n_a == static_cast<int>(a.size()));
  const population b(n_b, i_b);
  assert(n_b == static_cast<int>(b.size()));
  const population c(n_c, i_c);
  assert(n_c == static_cast<int>(c.size()));
  const population d(n_d, i_d);
  assert(n_d == static_cast<int>(d.size()));
  const population e(n_e, i_e);
  assert(n_e == static_cast<int>(e.size()));
  std::copy(std::begin(a), std::end(a), std::back_inserter(p));
  std::copy(std::begin(b), std::end(b), std::back_inserter(p));
  std::copy(std::begin(c), std::end(c), std::back_inserter(p));
  std::copy(std::begin(d), std::end(d), std::back_inserter(p));
  std::copy(std::begin(e), std::end(e), std::back_inserter(p));
  assert(n_a + n_b + n_c + n_d + n_e == static_cast<int>(p.size()));
  return p;
}

population population_factory::create_population_no_individuals() const noexcept
{
  return {};
}


population population_factory::create_population_one_individual() const noexcept
{
  return population{
    individual(individual::pin_t(""),individual::sil_t(2, 0b00))
  };
}

population population_factory::create_population_two_individuals_one_species() const noexcept
{
  return population{
    individual(individual::pin_t(""),individual::sil_t(2, 0b00)),
    individual(individual::pin_t(""),individual::sil_t(2, 0b01))
  };
}

population population_factory::create_population_two_individuals_two_species() const noexcept
{
  return population{
    individual(individual::pin_t(""),individual::sil_t(2, 0b00)),
    individual(individual::pin_t(""),individual::sil_t(2, 0b11))
  };
}

population population_factory::create_population_three_individuals_two_species() const noexcept
{
  return population{
    individual(individual::pin_t(""),individual::sil_t(2, 0b00)),
    individual(individual::pin_t(""),individual::sil_t(2, 0b00)),
    individual(individual::pin_t(""),individual::sil_t(2, 0b11))
  };
}

population population_factory::create_population_three_individuals_one_species_one_possible_species() const noexcept
{
  return population{
    individual(individual::pin_t(""),individual::sil_t(2, 0b00)),
    individual(individual::pin_t(""),individual::sil_t(2, 0b01)),
    individual(individual::pin_t(""),individual::sil_t(2, 0b11))
  };
}
