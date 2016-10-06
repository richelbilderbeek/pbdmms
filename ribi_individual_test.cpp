#include "distancer_individual.h"
#include <fstream>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include "distancer_helper.h"

BOOST_AUTO_TEST_CASE(test_create_offsping_individual_pin)
{
  const size_t n_pin_loci{4};
  const size_t n_sil_loci{1};
  const individual::sil_t sil(1, 0b0);
  const individual::pin_t pin_a(std::string(n_pin_loci, 'A'));
  const individual::pin_t pin_b(std::string(n_pin_loci, 'C'));
  {
    const individual p(pin_a, sil);
    const individual q(pin_b, sil);
    const boost::dynamic_bitset<> inherit_pin_from_p(n_pin_loci, 0b0000);
    const boost::dynamic_bitset<> inherit_sil_from_p(n_sil_loci, 0b0);
    const individual kid = create_offspring(p, q, inherit_pin_from_p, inherit_sil_from_p);
    const individual kid_should_be(
      individual::pin_t("CCCC"), sil
    );
    BOOST_CHECK(kid == kid_should_be);
  }
  {
    const individual p(pin_a, sil);
    const individual q(pin_b, sil);
    const boost::dynamic_bitset<> inherit_pin_from_p(n_pin_loci, 0b0101);
    const boost::dynamic_bitset<> inherit_sil_from_p(n_sil_loci, 0b0);
    const individual kid = create_offspring(p, q, inherit_pin_from_p, inherit_sil_from_p);
    const individual kid_should_be(
      individual::pin_t("CACA"), sil
    );
    BOOST_CHECK(kid == kid_should_be);
  }
  {
    const individual p(pin_a, sil);
    const individual q(pin_b, sil);
    const boost::dynamic_bitset<> inherit_pin_from_p(n_pin_loci, 0b1010);
    const boost::dynamic_bitset<> inherit_sil_from_p(n_sil_loci, 0b0);
    const individual kid = create_offspring(p, q, inherit_pin_from_p, inherit_sil_from_p);
    const individual kid_should_be(
      individual::pin_t("ACAC"), sil
    );
    BOOST_CHECK(kid == kid_should_be);
  }
  // Different PIN lengths
  {
    const individual p(individual::pin_t("AA"), sil);            //2
    const individual q(individual::pin_t("AAA"), sil);           //3
    const boost::dynamic_bitset<> inherit_pin_from_p(2, 0b1010); //2
    const boost::dynamic_bitset<> inherit_sil_from_p(n_sil_loci, 0b0);
    BOOST_CHECK_THROW(
      create_offspring(p, q, inherit_pin_from_p, inherit_sil_from_p),
      std::invalid_argument
    );
  }
  // PIN lengths do not match inherit_from_p length
  {
    const individual p(individual::pin_t("AAA"), sil);           //3
    const individual q(individual::pin_t("AAA"), sil);           //3
    const boost::dynamic_bitset<> inherit_pin_from_p(2, 0b1010); //2
    const boost::dynamic_bitset<> inherit_sil_from_p(n_sil_loci, 0b0);
    BOOST_CHECK_THROW(
      create_offspring(p, q, inherit_pin_from_p, inherit_sil_from_p),
      std::invalid_argument
    );
  }
}

BOOST_AUTO_TEST_CASE(test_create_offsping_individual_sil)
{
  const size_t n_pin_loci{2};
  const individual::pin_t pin(std::string(n_pin_loci, 'A'));
  {
    const size_t n_sil_loci{3};
    const individual p(pin, individual::sil_t(n_sil_loci, 0b001));
    const individual q(pin, individual::sil_t(n_sil_loci, 0b100));
    const boost::dynamic_bitset<> inherit_pin_from_p(n_pin_loci, 0);
    const boost::dynamic_bitset<> inherit_sil_from_p(n_sil_loci, 0b001);
    const individual kid = create_offspring(p, q, inherit_pin_from_p, inherit_sil_from_p);
    const individual kid_should_be(pin, individual::sil_t(n_sil_loci, 0b101));
    BOOST_CHECK(kid == kid_should_be);
  }
  {
    const size_t n_sil_loci{4};
    const individual p(pin, individual::sil_t(n_sil_loci, 0b0000));
    const individual q(pin, individual::sil_t(n_sil_loci, 0b1111));
    const boost::dynamic_bitset<> inherit_pin_from_p(n_pin_loci, 0);
    const boost::dynamic_bitset<> inherit_sil_from_p(n_sil_loci, 0b0101);
    const individual kid = create_offspring(p, q, inherit_pin_from_p, inherit_sil_from_p);
    const individual kid_should_be(pin, individual::sil_t(n_sil_loci, 0b1010));
    BOOST_CHECK(kid == kid_should_be);
  }
  {
    const size_t n_sil_loci{4};
    const individual p(pin, individual::sil_t(n_sil_loci, 0b1111));
    const individual q(pin, individual::sil_t(n_sil_loci, 0b0000));
    const boost::dynamic_bitset<> inherit_pin_from_p(n_pin_loci, 0);
    const boost::dynamic_bitset<> inherit_sil_from_p(n_sil_loci, 0b0101);
    const individual kid = create_offspring(p, q, inherit_pin_from_p, inherit_sil_from_p);
    const individual kid_should_be(pin, individual::sil_t(n_sil_loci, 0b0101));
    BOOST_CHECK(kid == kid_should_be);
  }
}

BOOST_AUTO_TEST_CASE(test_individual_default_construction_n_loci_and_sil_value)
{
  const individual::pin_t pin("AAAA");
  const size_t n_loci{8};
  const individual i(
    pin,
    individual::sil_t(n_loci, 0)
  );
  BOOST_CHECK(i.get_pin().size() == 4);
  BOOST_CHECK(i.get_sil().size() == n_loci);
  BOOST_CHECK(i.get_sil().to_ulong() == 0);
}

BOOST_AUTO_TEST_CASE(test_individual_construction_n_loci_and_sil_value)
{
  const size_t n_loci{8};
  const size_t sil_value{0b00101010};
  BOOST_CHECK(sil_value == 42); //What did you expect? :-)
  const individual::pin_t pin("AAAA");
  const individual i(
    pin, individual::sil_t(n_loci, sil_value)
  );
  BOOST_CHECK(i.get_sil().size() == n_loci);
  BOOST_CHECK(i.get_sil().to_ulong() == sil_value);
}

BOOST_AUTO_TEST_CASE(test_individual_construction_from_sil_t)
{
  const size_t n_sil_loci{8};
  const size_t sil_value{0b00101010};
  const individual::sil_t sil(n_sil_loci, sil_value);
  BOOST_CHECK(sil_value == 42); //What did you expect? :-)
  const individual::pin_t pin("AAAA");
  const individual i(pin, sil);
  BOOST_CHECK(i.get_sil().size() == n_sil_loci);
  BOOST_CHECK(i.get_sil().to_ulong() == sil_value);
}

BOOST_AUTO_TEST_CASE(test_individual_operator_equals)
{
  const size_t n_sil_loci{8};
  const individual::pin_t pin_a("AAAA");
  const individual::pin_t pin_b("CCCC");
  const individual::sil_t sil_a(n_sil_loci, 0b01);
  const individual::sil_t sil_b(n_sil_loci, 0b10);
  const individual a(pin_a, sil_a);
  const individual b(pin_a, sil_a);
  const individual c(pin_b, sil_b);
  BOOST_CHECK(a == a);
  BOOST_CHECK(a == b);
  BOOST_CHECK(a != c);
  BOOST_CHECK(b == a);
  BOOST_CHECK(b == b);
  BOOST_CHECK(b != c);
  BOOST_CHECK(c != a);
  BOOST_CHECK(c != b);
  BOOST_CHECK(c == c);
}

BOOST_AUTO_TEST_CASE(test_individual_operator_stream_out)
{
  const individual::pin_t pin("CCCC");
  const individual::sil_t sil(8, 0b00000000);
  const individual a(pin, sil);
  std::stringstream s;
  s << a;
  BOOST_CHECK(!s.str().empty());
}



BOOST_AUTO_TEST_CASE(test_count_abundances)
{
  const individual::pin_t pin("AAAA");
  {
    const individual::sil_t sil(3, 0b000);
    std::vector<individual> p {
      individual(pin, sil)
    };
    const std::vector<int> expected = { 1 };
    const auto result = ::count_abundances(p,1);
    BOOST_CHECK(result == expected);
  }
  {
    const individual::sil_t sil(3, 0b001);
    std::vector<individual> p {
      individual(pin, sil),
      individual(pin, sil)
    };
    const std::vector<int> expected = { 2 };
    const auto n_species = ::count_species(p,1);
    const auto result = ::count_abundances(p,1);
    BOOST_CHECK(n_species == 1);
    BOOST_CHECK(result == expected);
  }
  {
    const individual::sil_t sil(3, 0b010);
    std::vector<individual> p {
      individual(pin, sil),
      individual(pin, sil),
      individual(pin, sil)
    };
    const std::vector<int> expected = { 3 };
    const auto n_species = ::count_species(p,1);
    const auto result = ::count_abundances(p,1);
    BOOST_CHECK(n_species == 1);
    BOOST_CHECK(result == expected);
  }
  {
    std::vector<individual> p {
      individual(pin, individual::sil_t(3,0b100)),
      individual(pin, individual::sil_t(3,0b110)),
      individual(pin, individual::sil_t(3,0b010)),
      individual(pin, individual::sil_t(3,0b011)),
      individual(pin, individual::sil_t(3,0b001))
    };
    const std::vector<int> expected = { 5 };
    const auto result = ::count_abundances(p,1);
    BOOST_CHECK(result == expected);
  }
  {
    std::vector<individual> p {
      individual(pin, individual::sil_t(3,0b100)),
      individual(pin, individual::sil_t(3,0b001))
    };
    const std::vector<int> expected = { 1, 1 };
    const auto result = ::count_abundances(p,1);
    BOOST_CHECK(result == expected);
  }
}

BOOST_AUTO_TEST_CASE(count_species_boost)
{
  const individual::pin_t pin("AAAA");
  {
    std::vector<individual> p {
      individual(pin, individual::sil_t(3, 0b000))
    };
    BOOST_CHECK(::count_species(p,1) == 1);
  }
  {
    std::vector<individual> p {
      individual(pin, individual::sil_t(3, 0b001)),
      individual(pin, individual::sil_t(3, 0b001))
    };
    BOOST_CHECK(::count_species(p,1) == 1);
  }
  {
    std::vector<individual> p {
      individual(pin, individual::sil_t(3, 0b010)),
      individual(pin, individual::sil_t(3, 0b010)),
      individual(pin, individual::sil_t(3, 0b010))
    };
    BOOST_CHECK(::count_species(p,1) == 1);
  }
  {
    std::vector<individual> p {
      individual(pin, individual::sil_t(3, 0b100)),
      individual(pin, individual::sil_t(3, 0b110)),
      individual(pin, individual::sil_t(3, 0b010)),
      individual(pin, individual::sil_t(3, 0b011)),
      individual(pin, individual::sil_t(3, 0b001))
    };
    BOOST_CHECK(::count_species(p,1) == 1);
  }
  {
    std::vector<individual> p {
      individual(pin, individual::sil_t(3,0b100)),
      individual(pin, individual::sil_t(3,0b001))
    };
    BOOST_CHECK(::count_species(p,1) == 2);
  }
}

BOOST_AUTO_TEST_CASE(count_possible_species_test)
{
  const individual::pin_t pin("AAAA");
  {
    std::vector<individual> p {
      individual(pin, individual::sil_t(3, 0b000))
    };
    BOOST_CHECK(::count_possible_species(p,1) == 1);
  }
  {
    std::vector<individual> p {
      individual(pin, individual::sil_t(3, 0b001)),
      individual(pin, individual::sil_t(3, 0b011))
    };
    BOOST_CHECK(::count_possible_species(p,1) == 1);
  }
  {
    std::vector<individual> p {
      individual(pin, individual::sil_t(3, 0b001)),
      individual(pin, individual::sil_t(3, 0b011)),
      individual(pin, individual::sil_t(3, 0b010))
    };
    BOOST_CHECK(::count_possible_species(p,1) == 2);
  }
  {
    std::vector<individual> p {
      individual(pin, individual::sil_t(3, 0b100)),
      individual(pin, individual::sil_t(3, 0b110)),
      individual(pin, individual::sil_t(3, 0b010)),
      individual(pin, individual::sil_t(3, 0b011)),
      individual(pin, individual::sil_t(3, 0b001))
    };
    BOOST_CHECK(::count_possible_species(p,1) == 3);
  }
  {
    std::vector<individual> p {
      individual(pin, individual::sil_t(3,0b100)),
      individual(pin, individual::sil_t(3,0b001))
    };
    BOOST_CHECK(::count_possible_species(p,1) == 2);
  }
}

BOOST_AUTO_TEST_CASE(test_get_genetic_distance)
{
  const individual::pin_t pin("AAAA");
  individual a(pin, individual::sil_t(3, 0b000));
  individual b(pin, individual::sil_t(3, 0b001));
  individual c(pin, individual::sil_t(3, 0b011));
  BOOST_CHECK(::get_genetic_distance(a,a) == 0);
  BOOST_CHECK(::get_genetic_distance(b,b) == 0);
  BOOST_CHECK(::get_genetic_distance(a,b) == 1);
  BOOST_CHECK(::get_genetic_distance(b,a) == 1);
  BOOST_CHECK(::get_genetic_distance(c,a) == 2);
  BOOST_CHECK(::get_genetic_distance(a,c) == 2);
}
