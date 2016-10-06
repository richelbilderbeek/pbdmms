#include "jobo_parameters.h"
#include <iostream>

double create_init_parameters()
   {
    //set initial parameter values
    const double extinction_rate(0.1);
    const double mutation_rate(0.4);
    const double number_of_species(1);
    const double backward_mutation(0.4);
    const double mutation_steps(20);

    //show initial parameters to user
    std::cout<< "The initial parameter settings are: " << '\n';
    std::cout<< "Extinction rate: " << extinction_rate << '\n';
    std::cout<< "Mutation rate: " << mutation_rate << '\n';
    std::cout<< "Number of species: " << number_of_species << '\n';
    std::cout<< "Backward mutation rate: " << backward_mutation << '\n';
    std::cout<< "Mutation steps: " << mutation_steps << '\n';

return extinction_rate,mutation_rate,number_of_species,backward_mutation,mutation_steps;
}


