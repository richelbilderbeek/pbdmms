#ifndef SADO_PARAMETERS_H
#define SADO_PARAMETERS_H

#include <string>

namespace sado {

enum class erasure { erase, swap_trick };

class parameters
{
public:

  parameters(
    const int pop_size = 0,
    const erasure e = erasure::erase
  );


  auto get_erasure() const noexcept { return m_erasure; }

  auto get_pop_size() const noexcept { return m_pop_size; }

private:
  const erasure m_erasure;

  const int m_pop_size;
};

parameters readparameters(const std::string& filename);

} //~namespace sado

#endif // SADO_PARAMETERS_H
