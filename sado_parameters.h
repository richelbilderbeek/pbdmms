#ifndef SADO_PARAMETERS_H
#define SADO_PARAMETERS_H

namespace sado {

enum class erasure { erase, swap_trick };

class parameters
{
public:

  parameters(
    const int pop_size = 0,
    const erasure e = erasure::erase
  );

  int m_pop_size;

  auto get_erasure() const noexcept { return m_erasure; }

  auto get_pop_size() const noexcept { return m_pop_size; }

private:
  erasure m_erasure;
};

} //~namespace sado

#endif // SADO_PARAMETERS_H
