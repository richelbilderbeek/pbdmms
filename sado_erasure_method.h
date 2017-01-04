#ifndef SADO_ERASURE_METHOD_H
#define SADO_ERASURE_METHOD_H

#include <iosfwd>
#include <string>

namespace sado
{

enum class erasure_method
{
  erase,
  swap
};

erasure_method to_erasure_method(const std::string &s);

std::string to_str(const erasure_method m) noexcept;

std::ostream &operator<<(std::ostream &os, const erasure_method m) noexcept;

} //~namespace sado

#endif // SADO_ERASURE_METHOD_H
