#ifndef SADO_GAUSSER_IMPLEMENTATION_H
#define SADO_GAUSSER_IMPLEMENTATION_H

#include <string>

namespace sado {

/// * raw: just calculating the gaussian
/// * lut: using a look-up table
enum class gausser_implementation { raw, lut };

gausser_implementation to_gausser_implementation(const std::string& s);

} //~namespace sado

#endif // SADO_GAUSSER_IMPLEMENTATION_H
