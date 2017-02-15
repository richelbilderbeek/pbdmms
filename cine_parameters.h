#ifndef CINE_PARAMETERS_H
#define CINE_PARAMETERS_H

#include <string>
#include <vector>

class cine_parameters
{
public:
    cine_parameters(
    const int   generations = 3,
    const int   ncols =  10,
    const int   nrows = 10,
    const int   prey_pop = 25,
    const int   predator_pop = 25,
    const double prob_mutation_to_0 = 0.05,
    const double prob_mutation_to_rd = 0.025,
    const int   timesteps = 10,
    const double ANN_cost = -0.15,
    const std::vector<int> layer_nodes = {3, 3, 1, 1}

    );

    int get_generations() const noexcept;
    int get_ncols() const noexcept;
    int get_nrows() const noexcept;
    int get_prey_pop() const noexcept;
    int get_predator_pop() const noexcept;
    double get_prob_mutation_to_0() const noexcept;
    double get_prob_mutation_to_rd() const noexcept;
    int get_timesteps() const noexcept;
    double get_ANN_cost() const noexcept;
    std::vector<int> get_layer_nodes() const noexcept;






private:
     int   m_generations = 3;
     int   m_ncols = 10;
     int   m_nrows = 10;
     int   m_prey_pop = 25;
     int   m_predator_pop = 25;
     double m_prob_mutation_to_0 = 0.05;
     double m_prob_mutation_to_rd = 0.025;
     int   m_timesteps = 10;
     double m_ANN_cost = -0.15;
     std::vector<int> m_layer_nodes = {3, 3, 1, 1};


};


cine_parameters read_parameters_from_file(const std::string& filename);

extern cine_parameters g_parameters;

#endif // CINE_PARAMETERS_H
