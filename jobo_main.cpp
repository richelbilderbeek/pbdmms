#include "distancer_simulation.h"
#include <exception>
#include <iostream>
#include <fstream>
#include "distancer_helper.h"
#include "is_regular_file.h"
#include "convert_dot_to_svg.h"
#include "convert_svg_to_png.h"

int main() {
  try
  {
    const int max_genetic_distance{1};
    const int n_generations{100};
    const int n_pin_loci{0};
    const int n_sil_loci{4};
    const double pin_mutation_rate{0.1}; //Chance to have 1 locus flipped in a genome
    const int population_size{8};
    const std::string results_genotype_frequency_graph_filename{"results.dot"};
    const int rng_seed{30};
    const int sampling_interval{1};
    const double sil_mutation_rate{0.1}; //Chance to have 1 locus flipped in a genome
    const parameters p(
      max_genetic_distance,
      n_generations,
      n_pin_loci,
      n_sil_loci,
      pin_mutation_rate,
      population_size,
      results_genotype_frequency_graph_filename,
      rng_seed,
      sampling_interval,
      sil_mutation_rate
    );
    do_simulation(p);
    if (!is_regular_file(results_genotype_frequency_graph_filename))
    {
      std::cerr << "Error: file '" << results_genotype_frequency_graph_filename
        << "' not found" <<'\n';
      return 1;
    }
    convert_dot_to_svg(results_genotype_frequency_graph_filename, "results.svg");
    convert_svg_to_png("results.svg", "results.png");
    if (!"On local computer")
    {
      const int error{std::system("display results.png")};
      if (error)
      {
        std::cerr << "System error: " << error << '\n';
        return 1;
      }
    }
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
