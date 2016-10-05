#include "distancer_results.h"
#include <fstream>
#include <boost/test/unit_test.hpp>
#include <boost/graph/adjacency_list.hpp>
#include "distancer_population_factory.h"
#include "is_regular_file.h"
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"

// From a population, create a single node phylogeny:
//
// +----+
// | 00 | <- SIL
// | 02 | <- frequency
// +----+
BOOST_AUTO_TEST_CASE(test_results_create_single_node_phylogeny)
{
  const int max_genetic_distance{1};
  results r(max_genetic_distance);
  const pin my_pin("");
  const sil my_sil(2, 0b00);
  std::vector<individual> p{
    individual(my_pin, my_sil)
  };

  r.add_measurement(0, p);

  const auto g = r.get_sil_frequency_phylogeny();
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 1);
  BOOST_CHECK_EQUAL(g[*vertices(g).first].get_sil_frequencies().size(), 1);
}

/*

The initial state in data collection looks as such:

```
+--+--+--+--+--+--+--+--+--+--+
|G |t1|t2|t3|t4|t5|t6|t7|t8|t9|
+--+--+--+--+--+--+--+--+--+--+
|00| 9| 8| 6| 5| 4| 4| 4| 4| 4|
|01|  | 1| 2| 2| 2| 1|  |  |  |
|11|  |  | 1| 2| 3| 4| 5| 5| 5|
|10|  |  |  |  |  |  |  |  |  |
+--+--+--+--+--+--+--+--+--+--+
```

During measurements: create a graph from this, connecting the species:

```
     9--8--6--5--4--4--4--4--4
      \ |  |  |  |  | /
       -1--2--2--2--1
         \ |  |  |  | \
          -1--2--3--4--5--5--5

```

Next step: summarize genotypes:

 * One species possible: keep solid line
 * More species possible: use dotted line

```
           +--- speciation initiation
           |        +--- speciation completion
           |        |
           v        v
     9--9--9..9..9..9--4--4--4
                     \-5--5--5
```

Next step: summarize edges (the number above the edge denotes its length):

```
   1   1   3   1   2
 *---*->.*...*.<-*---*
             .
             . 1   2
             +.<-*---*
```

*/
BOOST_AUTO_TEST_CASE(test_results_example_complete_speciation)
{
  //+--+--+--+--+--+--+--+--+--+--+
  //|G |t1|t2|t3|t4|t5|t6|t7|t8|t9|
  //+--+--+--+--+--+--+--+--+--+--+
  //|00| 9| 8| 6| 5| 4| 4| 4| 4| 4|
  //|01|  | 1| 2| 2| 2| 1|  |  |  |
  //|11|  |  | 1| 2| 3| 4| 5| 5| 5|
  //|10|  |  |  |  |  |  |  |  |  |
  //+--+--+--+--+--+--+--+--+--+--+
  const individual i00(dna(""), sil(2,0b00));
  const individual i01(dna(""), sil(2,0b01));
  const individual i11(dna(""), sil(2,0b11));

  std::vector<population> populations{
    population_factory().create(9, i00, 0, i01, 0, i11),
    population_factory().create(8, i00, 1, i01, 0, i11),
    population_factory().create(6, i00, 2, i01, 1, i11),
    population_factory().create(5, i00, 2, i01, 2, i11),
    population_factory().create(4, i00, 2, i01, 3, i11),
    population_factory().create(4, i00, 1, i01, 4, i11),
    population_factory().create(4, i00, 0, i01, 5, i11),
    population_factory().create(4, i00, 0, i01, 5, i11),
    population_factory().create(4, i00, 0, i01, 5, i11)
  };
  assert(populations.size() == 9);
  const int n_populations{static_cast<int>(populations.size())};
  const int max_genetic_distance{1};
  results r(max_genetic_distance);
  for (int t=0; t!=n_populations; ++t)
  {
    assert(t >= 0 && t < static_cast<int>(populations.size()));
    r.add_measurement(t, populations[t]);
  }
  {
    const std::string filename_base{"test_results_example_complete_speciation_1"};
    const std::string filename_dot{filename_base + ".dot"};
    const std::string filename_svg{filename_base + ".svg"};
    const std::string filename_png{filename_base + ".png"};
    if (is_regular_file(filename_dot)) { std::remove(filename_dot.c_str()); }
    BOOST_CHECK(!is_regular_file(filename_dot));
    std::ofstream f(filename_dot);
    f << r.get_sil_frequency_phylogeny();
    BOOST_CHECK(is_regular_file(filename_dot));
    convert_dot_to_svg(filename_dot, filename_svg);
    convert_svg_to_png(filename_svg, filename_png);
  }
  r.summarize_sil_frequency_phylogeny();
  {
    const std::string filename_base{"test_results_example_complete_speciation_2"};
    const std::string filename_dot{filename_base + ".dot"};
    const std::string filename_svg{filename_base + ".svg"};
    const std::string filename_png{filename_base + ".png"};
    if (is_regular_file(filename_dot)) { std::remove(filename_dot.c_str()); }
    BOOST_CHECK(!is_regular_file(filename_dot));
    std::ofstream f(filename_dot);
    BOOST_TEST_PASSPOINT();
    f << r.get_summarized_sil_frequency_phylogeny();
    BOOST_TEST_PASSPOINT();
    BOOST_CHECK(is_regular_file(filename_dot));
    convert_dot_to_svg(filename_dot, filename_svg);
    convert_svg_to_png(filename_svg, filename_png);
  }
  const auto g = r.get_summarized_sil_frequency_phylogeny();
  /*
     1   1   3   1   2
   *---*->.*...*.<-*---*
               .
               . 1   2
               +.<-*---*
  */
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 8);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 7);
}

/*


This also allows for unsuccessfull speciation-initiations:

```
+--+--+--+--+--+--+--+--+--+--+
|G |t1|t2|t3|t4|t5|t6|t7|t8|t9|
+--+--+--+--+--+--+--+--+--+--+
|00| 9| 8| 6| 5| 4| 4| 5| 5| 4|
|01|  | 1| 2| 2| 2| 1| 2| 4| 5|
|11|  |  | 1| 2| 3| 4| 2|  |  |
|10|  |  |  |  |  |  |  |  |  |
+--+--+--+--+--+--+--+--+--+--+
```

Next step: create a graph from this:

```
     9--8--6--5--4--4--5--5--4
        1--2--2--2--1--2--4--5
           1--2--3--4--2

```

Next step: connect the species:

```
     9--8--6--5--4--4--5--5--4
        |  |  |  |  |  |  |  |
        1--2--2--2--1--2--4--5
           |  |  |  |  |
           1--2--3--4--2

```

Next step: summarize species:

 * One species possible: keep solid line
 * More species possible: use dotted line

```
           +--- speciation initiation
           |           +--- end of speciation-initiation
           |           |
           v           v
     9--9--9..9..9..9..9--9--9
```

Next step: summarize edges (the number above the edge denotes its length):

```
   1   1   4   1   1
 *---*->-*...*.>-*---*
```

*/
BOOST_AUTO_TEST_CASE(test_results_example_unsuccessfull_speciation)
{
  // +--+--+--+--+--+--+--+--+--+--+
  // |G |t1|t2|t3|t4|t5|t6|t7|t8|t9|
  // +--+--+--+--+--+--+--+--+--+--+
  // |00| 9| 8| 6| 5| 4| 4| 5|  |  |
  // |01|  | 1| 2| 2| 2| 1| 2| 4| 5|
  // |11|  |  | 1| 2| 3| 4| 2| 5| 4|
  // |10|  |  |  |  |  |  |  |  |  |
  // +--+--+--+--+--+--+--+--+--+--+

  const individual i00(dna(""), sil(2,0b00));
  const individual i01(dna(""), sil(2,0b01));
  const individual i11(dna(""), sil(2,0b11));

  std::vector<population> populations{
    population_factory().create(
      9, i00,
      0, i01,
      0, i11
    ),
    population_factory().create(
      8, i00,
      1, i01,
      0, i11
    ),
    population_factory().create(
      6, i00,
      2, i01,
      1, i11
    ),
    population_factory().create(
      5, i00,
      2, i01,
      2, i11
    ),
    population_factory().create(
      4, i00,
      2, i01,
      3, i11
    ),
    population_factory().create(
      4, i00,
      1, i01,
      4, i11
    ),
    population_factory().create(
      5, i00,
      2, i01,
      2, i11
    ),
    population_factory().create(
      0, i00,
      4, i01,
      5, i11
    ),
    population_factory().create(
      0, i00,
      5, i01,
      4, i11
    )
  };
  assert(populations.size() == 9);
  const int n_populations{static_cast<int>(populations.size())};
  const int max_genetic_distance{1};
  results r(max_genetic_distance);
  for (int t=0; t!=n_populations; ++t)
  {
    assert(t >= 0 && t < static_cast<int>(populations.size()));
    r.add_measurement(t, populations[t]);
  }
  {
    const std::string filename_base{"test_results_example_incomplete_speciation_1"};
    const std::string filename_dot{filename_base + ".dot"};
    const std::string filename_svg{filename_base + ".svg"};
    const std::string filename_png{filename_base + ".png"};
    if (is_regular_file(filename_dot)) { std::remove(filename_dot.c_str()); }
    BOOST_CHECK(!is_regular_file(filename_dot));
    std::ofstream f(filename_dot);
    f << r.get_sil_frequency_phylogeny();
    BOOST_CHECK(is_regular_file(filename_dot));
    convert_dot_to_svg(filename_dot, filename_svg);
    convert_svg_to_png(filename_svg, filename_png);
    //const std::string cmd{"display " + filename_png};
    //std::system(cmd.c_str());
  }
  r.summarize_sil_frequency_phylogeny();
  {
    const std::string filename_base{"test_results_example_incomplete_speciation_2"};
    const std::string filename_dot{filename_base + ".dot"};
    const std::string filename_svg{filename_base + ".svg"};
    const std::string filename_png{filename_base + ".png"};
    if (is_regular_file(filename_dot)) { std::remove(filename_dot.c_str()); }
    BOOST_CHECK(!is_regular_file(filename_dot));
    std::ofstream f(filename_dot);
    BOOST_TEST_PASSPOINT();
    f << r.get_summarized_sil_frequency_phylogeny();
    BOOST_TEST_PASSPOINT();
    BOOST_CHECK(is_regular_file(filename_dot));
    convert_dot_to_svg(filename_dot, filename_svg);
    convert_svg_to_png(filename_svg, filename_png);
    //const std::string cmd{"display " + filename_png};
    //std::system(cmd.c_str());
  }
  const auto g = r.get_summarized_sil_frequency_phylogeny();
 /*

   1   1   4   1   1
 *---*->-*...*.>-*---*

  */

  BOOST_CHECK_EQUAL(boost::num_vertices(g), 6);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 5);

}



/*

There is a problem case, I do not think is a problem:


```
+---+--+--+--+--+--+--+--+--+--+
| G |t1|t2|t3|t4|t5|t6|t7|t8|t9|
+---+--+--+--+--+--+--+--+--+--+
|000|  |  |  | 2| 2| 2| 2|  |  |
|001|  | 3| 3| 2| 2| 2| 2| 3|  |
|011| 9| 3| 3|  |  |  | 1| 3| 9|
|010|  | 3| 3| 3| 3| 3| 2| 3|  |
|110|  |  |  | 2| 2| 2| 2|  |  |
+---+--+--+--+--+--+--+--+--+--+
```

Next step: create a graph from this:

```
            2--2--2--2
      3--3--2--2--2--2--3
   9--3--3           1--3--9
      3--3--3--3--3--2--3
            2--2--2--2

```

Next step: connect the species:

```
            2--2--2--2
            |  |  |  |
      3--3--2--2--2--2--3
      |  |           |  |
   9--3--3           1--3--9
      |  |           |  |
      3--3--3--3--3--2--3
            |  |  |  |
            2--2--2--2

```

Next step: summarize species:

 * One species possible: keep solid line
 * More species possible: use dotted line

```
      +--- speciation initiation
      |  +--- end of speciation-initiation
      |  |
      v  v
   9--9..9--4--4--4--9--9--9
          \         /
           \       /
            5--5--5

```

Next step: smash loops, use an equals (`=`) line here:

```
      +--- speciation initiation
      |              +--- end of speciation-initiation
      |              |
      v              v
   9--9..9==9==9==9==9--9--9

```

An equals (`=`) line denotes: there were two species, that could reform the
missing link again. These occurrances should be rare for more loci! I
intend to treat these lineages as incipient species, as they could
not keep a good species established.

Next step: summarize edges (the number above the edge denotes its length):

```
    1   1   4   2
  9---9...9===9---9
```
*/
BOOST_AUTO_TEST_CASE(test_results_example_problem_case)
{
  // +---+--+--+--+--+--+--+--+--+--+
  // | G |t1|t2|t3|t4|t5|t6|t7|t8|t9|
  // +---+--+--+--+--+--+--+--+--+--+
  // |000|  |  |  | 2| 2| 2| 2|  |  |
  // |001|  | 3| 3| 2| 2| 2| 2| 3|  |
  // |011| 9| 3| 3|  |  |  | 1| 3| 9|
  // |111|  | 3| 3| 3| 3| 3| 2| 3|  |
  // |110|  |  |  | 2| 2| 2| 2|  |  |
  // +---+--+--+--+--+--+--+--+--+--+
  const individual i000(dna(""), sil(3,0b000));
  const individual i001(dna(""), sil(3,0b001));
  const individual i011(dna(""), sil(3,0b011));
  const individual i111(dna(""), sil(3,0b111));
  const individual i110(dna(""), sil(3,0b110));

  std::vector<population> populations{
    population_factory().create(0, i000, 0, i001, 9, i011, 0, i111, 0, i110),
    population_factory().create(0, i000, 3, i001, 3, i011, 3, i111, 0, i110),
    population_factory().create(0, i000, 3, i001, 3, i011, 3, i111, 0, i110),
    population_factory().create(2, i000, 2, i001, 0, i011, 3, i111, 2, i110),
    population_factory().create(2, i000, 2, i001, 0, i011, 3, i111, 2, i110),
    population_factory().create(2, i000, 2, i001, 0, i011, 3, i111, 2, i110),
    population_factory().create(2, i000, 2, i001, 1, i011, 2, i111, 2, i110),
    population_factory().create(0, i000, 3, i001, 3, i011, 3, i111, 0, i110),
    population_factory().create(0, i000, 0, i001, 9, i011, 0, i111, 0, i110)
  };
  assert(populations.size() == 9);
  const int n_populations{static_cast<int>(populations.size())};
  const int max_genetic_distance{1};
  results r(max_genetic_distance);
  for (int t=0; t!=n_populations; ++t)
  {
    assert(t >= 0 && t < static_cast<int>(populations.size()));
    r.add_measurement(t, populations[t]);
  }
  {
    const std::string filename_base{"test_results_example_problem_case_1"};
    const std::string filename_dot{filename_base + ".dot"};
    const std::string filename_svg{filename_base + ".svg"};
    const std::string filename_png{filename_base + ".png"};
    if (is_regular_file(filename_dot)) { std::remove(filename_dot.c_str()); }
    BOOST_CHECK(!is_regular_file(filename_dot));
    std::ofstream f(filename_dot);
    f << r.get_sil_frequency_phylogeny();
    BOOST_CHECK(is_regular_file(filename_dot));
    convert_dot_to_svg(filename_dot, filename_svg);
    convert_svg_to_png(filename_svg, filename_png);
  }
  r.summarize_sil_frequency_phylogeny();
  {
    const std::string filename_base{"test_results_example_problem_case_2"};
    const std::string filename_dot{filename_base + ".dot"};
    const std::string filename_svg{filename_base + ".svg"};
    const std::string filename_png{filename_base + ".png"};
    if (is_regular_file(filename_dot)) { std::remove(filename_dot.c_str()); }
    BOOST_CHECK(!is_regular_file(filename_dot));
    std::ofstream f(filename_dot);
    BOOST_TEST_PASSPOINT();
    f << r.get_summarized_sil_frequency_phylogeny();
    BOOST_TEST_PASSPOINT();
    BOOST_CHECK(is_regular_file(filename_dot));
    convert_dot_to_svg(filename_dot, filename_svg);
    convert_svg_to_png(filename_svg, filename_png);
  }
  const auto g = r.get_summarized_sil_frequency_phylogeny();
  /*
    1   1   4   2
  9---9...9===9---9

  */
  return;
  //TODO: will fail
  BOOST_CHECK_EQUAL(boost::num_vertices(g), 5);
  BOOST_CHECK_EQUAL(boost::num_edges(g), 4);
}
