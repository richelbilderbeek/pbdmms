#ifndef CINE_PARAMETERS_H
#define CINE_PARAMETERS_H

#include <string>
#include <vector>

// Hanno: plain struct should be fine.

// Christoph: With regard to robustness, could I also maintain the class?

class cine_parameters
{
public:
    cine_parameters(
    const int   generations = 3,
    const int   ncols =  10,
    const int   nrows = 10,
    const int   prey_pop = 25,
    const int   predator_pop = 1,
    const double prob_mutation_to_0 = 0.05,
    const double prob_mutation_to_rd = 0.025,
    const int   timesteps = 10,
    const double ANN_cost = -0.15,
    const std::vector<int> layer_nodes = {3, 3, 1}

    );

    int generations() const noexcept;
    int ncols() const noexcept;
    int nrows() const noexcept;
    int prey_pop() const noexcept;
    int predator_pop() const noexcept;
    double prob_mutation_to_0() const noexcept;
    double prob_mutation_to_rd() const noexcept;
    int timesteps() const noexcept;
    double ANN_cost() const noexcept;
    std::vector<int> layer_nodes() const noexcept;






private:
     int   m_generations = 3;
     int   m_ncols = 10;
     int   m_nrows = 10;
     int   m_prey_pop = 25;
     int   m_predator_pop = 1;
     double m_prob_mutation_to_0 = 0.05;
     double m_prob_mutation_to_rd = 0.025;
     int   m_timesteps = 10;
     double m_ANN_cost = -0.15;
     std::vector<int> m_layer_nodes = {3, 3, 1};


};


cine_parameters read_parameters_from_file(const std::string& filename);


#endif // CINE_PARAMETERS_H
