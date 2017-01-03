#ifndef SADO_NEXT_GENERATION_METHOD_H
#define SADO_NEXT_GENERATION_METHOD_H

#include <string>

namespace sado
{

/// The method with which the next generation
/// is calculated:
/// * overlapping: generations overlap, as in the original article
/// * seperate: generations are seperated
enum class next_generation_method
{
  overlapping,
  seperate
};

next_generation_method to_next_gen_method(const std::string &s);

} //~namespace sado

#endif // SADO_NEXT_GENERATION_METHOD_H
