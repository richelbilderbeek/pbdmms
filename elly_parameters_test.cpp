#include "elly_parameters.h"

#include <sstream>
#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/split.hpp>

// Boost.Test does not play well with -Weffc++
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"

using namespace elly;

BOOST_AUTO_TEST_CASE(elly_parameters_constructor_and_getters_must_match)
{
  const per_species_rate ana{0.3};
  const per_species_rate clado_is{0.2};
  const per_species_rate clado_main{0.1};
  const per_species_rate ext_is{0.4};
  const per_species_rate ext_main{0.5};
  const per_species_rate mig_to_is{0.6};
  const per_species_rates rates(
    ana,
    clado_is,
    clado_main,
    ext_is,
    ext_main,
    mig_to_is
  );
  const carrying_capacity carryingcap_is{10};
  const carrying_capacity carryingcap_main{20};
  const unsigned int rng_seed{3};
  const int init_n_main_cls{4};
  const int init_n_main_sps{5};
  const double crown_age{0.7};
  const parameters a(
    rates,
    carryingcap_is,
    carryingcap_main,
    rng_seed,
    init_n_main_cls,
    init_n_main_sps,
    crown_age
  );
  BOOST_CHECK_EQUAL(a.get_carryingcap_is(), carryingcap_is);
  BOOST_CHECK_EQUAL(a.get_carryingcap_main(), carryingcap_main);
  BOOST_CHECK_EQUAL(a.get_crown_age(), crown_age);
  BOOST_CHECK_EQUAL(a.get_init_n_main_cls(), init_n_main_cls);
  BOOST_CHECK_EQUAL(a.get_init_n_main_sps(), init_n_main_sps);
  BOOST_CHECK_EQUAL(a.get_rates(), rates);
  BOOST_CHECK_EQUAL(a.get_rng_seed(), rng_seed);

}


BOOST_AUTO_TEST_CASE(elly_parameters_settings)
{
  const parameters a = create_parameters_set1();
  const parameters b = create_parameters_set1();
  BOOST_CHECK(a.get_ana_rate() == b.get_ana_rate());

}

BOOST_AUTO_TEST_CASE(elly_all_parameters_settings)
{
  const parameters a = create_parameters_set1();
  const parameters b = create_parameters_set1();
  const parameters c = create_parameters_set2();
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

BOOST_AUTO_TEST_CASE(elly_create_parameters_sets)
{
  BOOST_CHECK_NO_THROW(create_parameters_set1());
  BOOST_CHECK_NO_THROW(create_parameters_set2());
  BOOST_CHECK_NO_THROW(create_parameters_set3());
  BOOST_CHECK_NO_THROW(create_profiling_parameters());
}

BOOST_AUTO_TEST_CASE(elly_parameters_operator_stream_out)
{
  const auto p = create_parameters_set1();
  std::stringstream s;
  s << p;
  BOOST_CHECK(!s.str().empty());
}

BOOST_AUTO_TEST_CASE(elly_parameters_operator_streaming_once)
{
  const per_species_rate ana{0.3};
  const per_species_rate clado_is{0.2};
  const per_species_rate clado_main{0.1};
  const per_species_rate ext_is{0.4};
  const per_species_rate ext_main{0.5};
  const per_species_rate mig_to_is{0.6};
  const per_species_rates rates(
    ana,
    clado_is,
    clado_main,
    ext_is,
    ext_main,
    mig_to_is
  );

  const carrying_capacity carryingcap_is{10};
  const carrying_capacity carryingcap_main{20};
  const unsigned int rng_seed{3};
  const int init_n_main_cls{4};
  const int init_n_main_sps{5};
  const double crown_age{0.7};
  const parameters a(
    rates,
    carryingcap_is,
    carryingcap_main,
    rng_seed,
    init_n_main_cls,
    init_n_main_sps,
    crown_age
  );

  parameters b = create_parameters_set2();
  assert(a != b);
  std::stringstream s;
  s << a;
  s >> b;
  assert(b != create_parameters_set2());
  BOOST_CHECK_EQUAL(a.get_ana_rate(), ana);
  BOOST_CHECK_EQUAL(a.get_carryingcap_is(), carryingcap_is);
  BOOST_CHECK_EQUAL(a.get_carryingcap_main(), carryingcap_main);
  BOOST_CHECK_EQUAL(a.get_clado_is(), clado_is);
  BOOST_CHECK_EQUAL(a.get_clado_main(), clado_main);
  BOOST_CHECK_EQUAL(a.get_crown_age(), crown_age);
  BOOST_CHECK_EQUAL(a.get_ext_is(), ext_is);
  BOOST_CHECK_EQUAL(a.get_ext_main(), ext_main);
  BOOST_CHECK_EQUAL(a.get_init_n_main_cls(), init_n_main_cls);
  BOOST_CHECK_EQUAL(a.get_init_n_main_sps(), init_n_main_sps);
  BOOST_CHECK_EQUAL(a.get_mig_to_island(), mig_to_is);
  BOOST_CHECK_EQUAL(a.get_rng_seed(), rng_seed);
  BOOST_CHECK_EQUAL(a,b);
}

BOOST_AUTO_TEST_CASE(elly_parameters_operator_streaming_twice)
{
  const parameters a = create_parameters_set2();
  const parameters b = create_parameters_set3();
  parameters c = create_parameters_set1();
  parameters d = create_parameters_set1();
  assert(a != b);
  assert(b != d);
  std::stringstream s;
  s << a << ' ' << b;
  s >> c >> d;
  BOOST_CHECK_EQUAL(a,c);
  BOOST_CHECK_EQUAL(b,d);
}

BOOST_AUTO_TEST_CASE(elly_parameters_save_and_load_must_be_symmetrical)
{
  {
    const std::string filename{
      "elly_parameters_save_and_load_must_be_symmetrical_1.txt"};
    const parameters p = create_parameters_set1();
    save(p, filename);
    const parameters q = load_parameters_from_file(filename);
    BOOST_CHECK_EQUAL(p, q);
  }
  {
    const std::string filename{
      "elly_parameters_save_and_load_must_be_symmetrical_2.txt"};
    const parameters p = create_parameters_set2();
    save(p, filename);
    const parameters q = load_parameters_from_file(filename);
    BOOST_CHECK_EQUAL(p, q);
  }
  {
    const std::string filename{
      "elly_parameters_save_and_load_must_be_symmetrical_3.txt"};
    const parameters p = create_parameters_set3();
    save(p, filename);
    const parameters q = load_parameters_from_file(filename);
    BOOST_CHECK_EQUAL(p, q);
  }
  {
    const std::string filename{
      "elly_parameters_save_and_load_must_be_symmetrical_profiling.txt"};
    const parameters p = create_profiling_parameters();
    save(p, filename);
    const parameters q = load_parameters_from_file(filename);
    BOOST_CHECK_EQUAL(p, q);
  }
}

BOOST_AUTO_TEST_CASE(elly_parameters_must_add_unit)
{
  //Correctly formed
  {
    std::stringstream s;
    s
      << "carryingcap_is: 30 species" << '\n'
      << "carryingcap_main: 10 species" << '\n'
      << "crown_age: 10" << '\n'
      << "init_n_main_cls: 8" << '\n'
      << "init_n_main_sps: 10" << '\n'
      << "ana: 0.87 per species per time unit" << '\n'
      << "clado_is: 0.2 per species per time unit" << '\n'
      << "clado_main: 0.2 per species per time unit" << '\n'
      << "ext_is: 0.1 per species per time unit" << '\n'
      << "ext_main: 0.1 per species per time unit" << '\n'
      << "mig_to_is: 0.02 per species per time unit" << '\n'
      << "rng_seed: 385" << '\n'
    ;
    parameters p = create_parameters_set1();
    s >> p;
    BOOST_CHECK_NE(p, create_parameters_set1());
  }
  //carryingcap_is: is misspelled
  {
    std::stringstream s;
    s
      << "carryingcapMISSPELLED_is: 30 species" << '\n'
      << "carryingcap_main: 10 species" << '\n'
      << "crown_age: 10" << '\n'
      << "init_n_main_cls: 8" << '\n'
      << "init_n_main_sps: 10" << '\n'
      << "ana: 0.87 per species per time unit" << '\n'
      << "clado_is: 0.2 per species per time unit" << '\n'
      << "clado_main: 0.2 per species per time unit" << '\n'
      << "ext_is: 0.1 per species per time unit" << '\n'
      << "ext_main: 0.1 per species per time unit" << '\n'
      << "mig_to_is: 0.02 per species per time unit" << '\n'
      << "rng_seed: 385" << '\n'
    ;
    parameters p = create_parameters_set1();
    BOOST_CHECK_THROW(s >> p, std::invalid_argument);
  }
  //carryingcap_main: is misspelled
  {
    std::stringstream s;
    s
      << "carryingcap_is: 30 species" << '\n'
      << "carryingcap_maiMISSPELLEDn: 10 species" << '\n'
      << "crown_age: 10" << '\n'
      << "init_n_main_cls: 8" << '\n'
      << "init_n_main_sps: 10" << '\n'
      << "ana: 0.87 per species per time unit" << '\n'
      << "clado_is: 0.2 per species per time unit" << '\n'
      << "clado_main: 0.2 per species per time unit" << '\n'
      << "ext_is: 0.1 per species per time unit" << '\n'
      << "ext_main: 0.1 per species per time unit" << '\n'
      << "mig_to_is: 0.02 per species per time unit" << '\n'
      << "rng_seed: 385" << '\n'
    ;
    parameters p = create_parameters_set1();
    BOOST_CHECK_THROW(s >> p, std::invalid_argument);
  }
  //crown_age: is misspelled
  {
    std::stringstream s;
    s
      << "carryingcap_is: 30 species" << '\n'
      << "carryingcap_main: 10 species" << '\n'
      << "crown_agMISSPELLEDe: 10" << '\n'
      << "init_n_main_cls: 8" << '\n'
      << "init_n_main_sps: 10" << '\n'
      << "ana: 0.87 per species per time unit" << '\n'
      << "clado_is: 0.2 per species per time unit" << '\n'
      << "clado_main: 0.2 per species per time unit" << '\n'
      << "ext_is: 0.1 per species per time unit" << '\n'
      << "ext_main: 0.1 per species per time unit" << '\n'
      << "mig_to_is: 0.02 per species per time unit" << '\n'
      << "rng_seed: 385" << '\n'
    ;
    parameters p = create_parameters_set1();
    BOOST_CHECK_THROW(s >> p, std::invalid_argument);
  }
  //init_n_main_cls: is misspelled
  {
    std::stringstream s;
    s
      << "carryingcap_is: 30 species" << '\n'
      << "carryingcap_main: 10 species" << '\n'
      << "crown_age: 10" << '\n'
      << "init_n_main_clMISSPELLEDs: 8" << '\n'
      << "init_n_main_sps: 10" << '\n'
      << "ana: 0.87 per species per time unit" << '\n'
      << "clado_is: 0.2 per species per time unit" << '\n'
      << "clado_main: 0.2 per species per time unit" << '\n'
      << "ext_is: 0.1 per species per time unit" << '\n'
      << "ext_main: 0.1 per species per time unit" << '\n'
      << "mig_to_is: 0.02 per species per time unit" << '\n'
      << "rng_seed: 385" << '\n'
    ;
    parameters p = create_parameters_set1();
    BOOST_CHECK_THROW(s >> p, std::invalid_argument);
  }
  //init_n_main_sps: is misspelled
  {
    std::stringstream s;
    s
      << "carryingcap_is: 30 species" << '\n'
      << "carryingcap_main: 10 species" << '\n'
      << "crown_age: 10" << '\n'
      << "init_n_main_cls: 8" << '\n'
      << "init_n_main_spMISSPELLEDs: 10" << '\n'
      << "ana: 0.87 per species per time unit" << '\n'
      << "clado_is: 0.2 per species per time unit" << '\n'
      << "clado_main: 0.2 per species per time unit" << '\n'
      << "ext_is: 0.1 per species per time unit" << '\n'
      << "ext_main: 0.1 per species per time unit" << '\n'
      << "mig_to_is: 0.02 per species per time unit" << '\n'
      << "rng_seed: 385" << '\n'
    ;
    parameters p = create_parameters_set1();
    BOOST_CHECK_THROW(s >> p, std::invalid_argument);
  }
  //ana: is misspelled
  {
    std::stringstream s;
    s
      << "carryingcap_is: 30 species" << '\n'
      << "carryingcap_main: 10 species" << '\n'
      << "crown_age: 10" << '\n'
      << "init_n_main_cls: 8" << '\n'
      << "init_n_main_sps: 10" << '\n'
      << "anMISSPELLEDa: 0.87 per species per time unit" << '\n'
      << "clado_is: 0.2 per species per time unit" << '\n'
      << "clado_main: 0.2 per species per time unit" << '\n'
      << "ext_is: 0.1 per species per time unit" << '\n'
      << "ext_main: 0.1 per species per time unit" << '\n'
      << "mig_to_is: 0.02 per species per time unit" << '\n'
      << "rng_seed: 385" << '\n'
    ;
    parameters p = create_parameters_set1();
    BOOST_CHECK_THROW(s >> p, std::invalid_argument);
  }
  //clado_is: is misspelled
  {
    std::stringstream s;
    s
      << "carryingcap_is: 30 species" << '\n'
      << "carryingcap_main: 10 species" << '\n'
      << "crown_age: 10" << '\n'
      << "init_n_main_cls: 8" << '\n'
      << "init_n_main_sps: 10" << '\n'
      << "ana: 0.87 per species per time unit" << '\n'
      << "clado_MISSPELLEDis: 0.2 per species per time unit" << '\n'
      << "clado_main: 0.2 per species per time unit" << '\n'
      << "ext_is: 0.1 per species per time unit" << '\n'
      << "ext_main: 0.1 per species per time unit" << '\n'
      << "mig_to_is: 0.02 per species per time unit" << '\n'
      << "rng_seed: 385" << '\n'
    ;
    parameters p = create_parameters_set1();
    BOOST_CHECK_THROW(s >> p, std::invalid_argument);
  }
  //clado_main: is misspelled
  {
    std::stringstream s;
    s
      << "carryingcap_is: 30 species" << '\n'
      << "carryingcap_main: 10 species" << '\n'
      << "crown_age: 10" << '\n'
      << "init_n_main_cls: 8" << '\n'
      << "init_n_main_sps: 10" << '\n'
      << "ana: 0.87 per species per time unit" << '\n'
      << "clado_is: 0.2 per species per time unit" << '\n'
      << "clMISSPELLEDado_main: 0.2 per species per time unit" << '\n'
      << "ext_is: 0.1 per species per time unit" << '\n'
      << "ext_main: 0.1 per species per time unit" << '\n'
      << "mig_to_is: 0.02 per species per time unit" << '\n'
      << "rng_seed: 385" << '\n'
    ;
    parameters p = create_parameters_set1();
    BOOST_CHECK_THROW(s >> p, std::invalid_argument);
  }
  //ext_is: is misspelled
  {
    std::stringstream s;
    s
      << "carryingcap_is: 30 species" << '\n'
      << "carryingcap_main: 10 species" << '\n'
      << "crown_age: 10" << '\n'
      << "init_n_main_cls: 8" << '\n'
      << "init_n_main_sps: 10" << '\n'
      << "ana: 0.87 per species per time unit" << '\n'
      << "clado_is: 0.2 per species per time unit" << '\n'
      << "clado_main: 0.2 per species per time unit" << '\n'
      << "eMISSPELLEDxt_is: 0.1 per species per time unit" << '\n'
      << "ext_main: 0.1 per species per time unit" << '\n'
      << "mig_to_is: 0.02 per species per time unit" << '\n'
      << "rng_seed: 385" << '\n'
    ;
    parameters p = create_parameters_set1();
    BOOST_CHECK_THROW(s >> p, std::invalid_argument);
  }
  //ext_main: is misspelled
  {
    std::stringstream s;
    s
      << "carryingcap_is: 30 species" << '\n'
      << "carryingcap_main: 10 species" << '\n'
      << "crown_age: 10" << '\n'
      << "init_n_main_cls: 8" << '\n'
      << "init_n_main_sps: 10" << '\n'
      << "ana: 0.87 per species per time unit" << '\n'
      << "clado_is: 0.2 per species per time unit" << '\n'
      << "clado_main: 0.2 per species per time unit" << '\n'
      << "ext_is: 0.1 per species per time unit" << '\n'
      << "ext_mMISSPELLEDain: 0.1 per species per time unit" << '\n'
      << "mig_to_is: 0.02 per species per time unit" << '\n'
      << "rng_seed: 385" << '\n'
    ;
    parameters p = create_parameters_set1();
    BOOST_CHECK_THROW(s >> p, std::invalid_argument);
  }
  //mig_to_is: is misspelled
  {
    std::stringstream s;
    s
      << "carryingcap_is: 30 species" << '\n'
      << "carryingcap_main: 10 species" << '\n'
      << "crown_age: 10" << '\n'
      << "init_n_main_cls: 8" << '\n'
      << "init_n_main_sps: 10" << '\n'
      << "ana: 0.87 per species per time unit" << '\n'
      << "clado_is: 0.2 per species per time unit" << '\n'
      << "clado_main: 0.2 per species per time unit" << '\n'
      << "ext_is: 0.1 per species per time unit" << '\n'
      << "ext_main: 0.1 per species per time unit" << '\n'
      << "miMISSPELLEDg_to_is: 0.02 per species per time unit" << '\n'
      << "rng_seed: 385" << '\n'
    ;
    parameters p = create_parameters_set1();
    BOOST_CHECK_THROW(s >> p, std::invalid_argument);
  }
  //rng_seed: is misspelled
  {
    std::stringstream s;
    s
      << "carryingcap_is: 30 species" << '\n'
      << "carryingcap_main: 10 species" << '\n'
      << "crown_age: 10" << '\n'
      << "init_n_main_cls: 8" << '\n'
      << "init_n_main_sps: 10" << '\n'
      << "ana: 0.87 per species per time unit" << '\n'
      << "clado_is: 0.2 per species per time unit" << '\n'
      << "clado_main: 0.2 per species per time unit" << '\n'
      << "ext_is: 0.1 per species per time unit" << '\n'
      << "ext_main: 0.1 per species per time unit" << '\n'
      << "mig_to_is: 0.02 per species per time unit" << '\n'
      << "rMISSPELLEDng_seed: 385" << '\n'
    ;
    parameters p = create_parameters_set1();
    BOOST_CHECK_THROW(s >> p, std::invalid_argument);
  }

}

#pragma GCC diagnostic pop
