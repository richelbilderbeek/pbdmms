///Intelligence in Predator Prey interactions - Christoph Netz
/// Preliminary script, 13 Dec 2016


#include "cine_individual.h"
#include "cine_simulation.h"
#include "cine_parameters.h"
#include <initializer_list>

int main(int argc, char *argv[])
{
    for (int i=0; i!=argc; ++i)
      {
        std::cout << i << ": " << argv[i] << '\n';
    }

    cine_parameters p;

    if (argc == 2)
      {
        if (std::string(argv[1]) == std::string("--profile"))
    {
        const cine_parameters q(
                    3,      //generations
                    10,     //ncols
                    10,     //nrows
                    25,     //prey_pop
                    25,     //predator_pop
                    0.05,   //prob_mutation_to_0
                    0.025,   //prob_mutation_to_rd
                    10      //timesteps
                    );
        p = q;
        g_parameters = q;

        }
        else
        {
            const std::string filename = argv[1];
                  p = read_parameters_from_file(filename);
            g_parameters = p;
        }
    }

    else
    {
        g_parameters = p;
    }

    do_simulation(p.get_generations(),
                  p.get_ncols(), p.get_nrows(),
                  p.get_prey_pop(),
                  p.get_predator_pop(),
                  p.get_prob_mutation_to_0(),
                  p.get_prob_mutation_to_rd(),
                  p.get_timesteps());
}




