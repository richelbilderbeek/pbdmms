#ifndef PBD_NLTT_H
#define PBD_NLTT_H

#include <string>
#include <vector>

namespace pbd {

using nltt = std::vector<std::pair<double, double>>;

nltt load_nltt_from_csv(const std::string& csv_filename);

} //~namespace pbd

#endif // PBD_NLTT_H
