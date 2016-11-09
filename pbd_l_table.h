#ifndef PBD_L_TABLE_H
#define PBD_L_TABLE_H

#include <iosfwd>
#include <vector>
#include "pbd_l_table_row.h"

///PBD functions
namespace pbd {

///A PBD L table
class l_table
{
public:
  l_table();
  const auto& get() const noexcept { return m_data; }
  void push_back(const l_table_row& row);
  auto size() const noexcept { return m_data.size(); }

private:

  std::vector<l_table_row> m_data;
};

l_table load_l_table_from_csv(const std::string& csv_filename);

//std::ifstream& operator>>(std::ifstream& is, l_table& t);
std::ofstream& operator<<(std::ifstream& is, l_table& t);

} //~namespace pbd

#endif // PBD_L_TABLE_H
