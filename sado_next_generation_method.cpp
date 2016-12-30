#include "sado_next_generation_method.h"
#include <stdexcept>

sado::next_generation_method sado::to_next_gen_method(const std::string &s) {
  if (s == "overlapping")
    return next_generation_method::overlapping;
  if (s == "seperate")
    return next_generation_method::seperate;
  throw std::invalid_argument(
      "cannot convert string to next_generation_method");
}
