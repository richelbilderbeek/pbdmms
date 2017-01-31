#include "cine_parameters.h"

#include <cassert>
#include <iostream>


cine_parameters g_parameters;

cine_parameters::cine_parameters(
        const int generations,
        const int ncols,
        const int nrows,
        const int prey_pop,
        const int predator_pop,
        const float prob_mutation_to_0,
        float prob_mutation_to_rd,
        const int timesteps
        )
        :
          m_generations         {generations},
          m_ncols               {ncols},
          m_nrows               {nrows},
          m_prey_pop            {prey_pop},
          m_predator_pop        {predator_pop},
          m_prob_mutation_to_0  {prob_mutation_to_0},
          m_prob_mutation_to_rd {prob_mutation_to_rd},
          m_timesteps           {timesteps}
{
    assert(m_generations >= 0);
    assert(m_ncols >= 0);
    assert(m_nrows >= 0);
    assert(m_prey_pop >= 0);
    assert(m_predator_pop >= 0);
    assert(m_prob_mutation_to_0 >= 0);
    assert(m_prob_mutation_to_rd >= 0);
    assert(m_timesteps >= 0);
}

int cine_parameters::get_generations() const noexcept
{
    return this->m_generations;
}

int cine_parameters::get_ncols() const noexcept
{
    return this->m_ncols;
}

int cine_parameters::get_nrows() const noexcept
{
    return this->m_nrows;
}

int cine_parameters::get_prey_pop() const noexcept
{
    return this->m_prey_pop;
}

int cine_parameters::get_predator_pop() const noexcept
{
    return this->m_predator_pop;
}

float cine_parameters::get_prob_mutation_to_0() const noexcept
{
    return this->m_prob_mutation_to_0;
}

float cine_parameters::get_prob_mutation_to_rd() const noexcept
{
    return this->m_prob_mutation_to_rd;
}

int cine_parameters::get_timesteps() const noexcept
{
    return this->m_timesteps;
}


cine_parameters read_parameters_from_file(const std::string& filename)
{
  std::clog << "STUB! read_parameters_from_file: " << filename << '\n';
  return cine_parameters();
}
