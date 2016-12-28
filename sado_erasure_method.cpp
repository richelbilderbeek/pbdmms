#include "sado_erasure_method.h"

#include <stdexcept>

sado::erasure_method sado::to_erasure_method(const std::string& s)
{
  if (s == "erase") return erasure_method::erase;
  if (s == "swap") return erasure_method::swap;
  throw std::invalid_argument("cannot convert string to erasure");
}
