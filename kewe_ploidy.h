#ifndef KEWE_PLOIDY_H
#define KEWE_PLOIDY_H

#include <iosfwd>
#include <string>

namespace kewe {

enum class ploidy { haploid, diploid };

std::string to_string(const ploidy p) noexcept;

std::ostream& operator<<(std::ostream& os, const ploidy p) noexcept;

} //~namespace kewe {

#endif // KEWE_PLOIDY_H
