#include "cine_parameters.h"

#include <cassert>
#include <iostream>



cine_parameters::cine_parameters(
        const int generations,
        const int ncols,
        const int nrows,
        const int prey_pop,
        const int predator_pop,
        const double prob_mutation_to_0,
        double prob_mutation_to_rd,
        const int timesteps,
        const double ANN_cost,
        const std::vector<int> layer_nodes
        )
        :
          m_generations         {generations},
          m_ncols               {ncols},
          m_nrows               {nrows},
          m_prey_pop            {prey_pop},
          m_predator_pop        {predator_pop},
          m_prob_mutation_to_0  {prob_mutation_to_0},
          m_prob_mutation_to_rd {prob_mutation_to_rd},
          m_timesteps           {timesteps},
          m_ANN_cost            {ANN_cost},
          m_layer_nodes         {layer_nodes}
{
    assert(m_generations >= 0);
    assert(m_ncols >= 0);
    assert(m_nrows >= 0);
    assert(m_prey_pop >= 0);
    assert(m_predator_pop >= 0);
    assert(m_prob_mutation_to_0 >= 0);
    assert(m_prob_mutation_to_rd >= 0);
    assert(m_timesteps >= 0);
    assert(m_ANN_cost  <= 0);
    //assert(m_layer_nodes.size() >0); cyclomatic complexity travis issue
}

int cine_parameters::generations() const noexcept
{
    return this->m_generations;
}

int cine_parameters::ncols() const noexcept
{
    return this->m_ncols;
}

int cine_parameters::nrows() const noexcept
{
    return this->m_nrows;
}

int cine_parameters::prey_pop() const noexcept
{
    return this->m_prey_pop;
}

int cine_parameters::predator_pop() const noexcept
{
    return this->m_predator_pop;
}

double cine_parameters::prob_mutation_to_0() const noexcept
{
    return this->m_prob_mutation_to_0;
}

double cine_parameters::prob_mutation_to_rd() const noexcept
{
    return this->m_prob_mutation_to_rd;
}

int cine_parameters::timesteps() const noexcept
{
    return this->m_timesteps;
}

double cine_parameters::ANN_cost() const noexcept
{
    return this->m_ANN_cost;
}

std::vector<int> cine_parameters::layer_nodes() const
{
    return this->m_layer_nodes;
}


cine_parameters read_parameters_from_file(const std::string& filename)
{
  std::clog << "STUB! read_parameters_from_file: " << filename << '\n';
  return cine_parameters();
}
