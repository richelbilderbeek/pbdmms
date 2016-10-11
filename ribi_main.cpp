#include "ribi_simulation.h"
#include <exception>
#include <iostream>
#include <fstream>
#include "ribi_helper.h"
#include "is_regular_file.h"
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"

ribi::parameters create_parameters()
{
  const int max_genetic_distance{1};
  const int n_generations{100};
  const int n_pin_loci{0};
  const int n_sil_loci{4};
  const double pin_mutation_rate{0.1}; //Chance to have 1 locus flipped in a genome
  const int population_size{8};
  const std::string rgfg_filename{"results.dot"}; //results_genotype_frequency_graph_filename
  const int rng_seed{30};
  const int sampling_interval{1};
  const double sil_mutation_rate{0.1}; //Chance to have 1 locus flipped in a genome
  return ribi::parameters(
    max_genetic_distance,
    n_generations,
    n_pin_loci,
    n_sil_loci,
    pin_mutation_rate,
    population_size,
    rgfg_filename,
    rng_seed,
    sampling_interval,
    sil_mutation_rate
  );
}

void process_results(const ribi::parameters& p)
{

  if (!is_regular_file(p.get_filename_genotype_frequency_graph()))
  {
    throw std::runtime_error("results file not found");
  }
  convert_dot_to_svg(p.get_filename_genotype_frequency_graph(), "results.svg");
  convert_svg_to_png("results.svg", "results.png");
}

int main()
{
  try
  {
    const auto p = create_parameters();
    ribi::do_simulation(p);
    process_results(p);
  }
  catch (std::exception& e)
  {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
  catch (...)
  {
    std::cerr << "Error: Unknown\n";
    return 1;
  }
}
