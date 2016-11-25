#ifndef PBD_NLTT_H
#define PBD_NLTT_H

#include <iosfwd>
#include <string>
#include <vector>

namespace pbd {

using nltt = std::vector<std::pair<double, double>>;

nltt create_test_nltt() noexcept;

nltt load_nltt_from_csv(const std::string& csv_filename);

std::ostream& operator<<(std::ostream& os, const nltt& p) noexcept;

} //~namespace pbd

#endif // PBD_NLTT_H
