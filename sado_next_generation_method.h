#ifndef SADO_NEXT_GENERATION_METHOD_H
#define SADO_NEXT_GENERATION_METHOD_H

#include <iosfwd>
#include <string>
#include <vector>

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

std::vector<next_generation_method> collect_all_next_generation_methods() noexcept;

next_generation_method to_next_generation_method(const std::string& s);

std::string to_str(const next_generation_method e) noexcept;

std::ostream& operator<<(std::ostream& os, const next_generation_method e) noexcept;

} //~namespace sado

#endif // SADO_NEXT_GENERATION_METHOD_H
