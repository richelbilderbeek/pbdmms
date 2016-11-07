#ifndef PBD_HELPER_H
#define PBD_HELPER_H

#include <string>
#include <vector>

///PBD functions
namespace pbd {

void delete_file(const std::string& filename);

std::vector<std::string> file_to_vector(const std::string& filename);

bool is_regular_file(const std::string& filename) noexcept;


} //~namespace pbd

#endif // PBD_HELPER_H
