#ifndef PBD_L_TABLE_H
#define PBD_L_TABLE_H

#include <iostream>

///PBD functions
namespace pbd {

///A PBD L table
class l_table
{
public:
  l_table();
};

l_table load_l_table_from_csv(const std::string& csv_filename);

//std::ifstream& operator>>(std::ifstream& is, l_table& t);
std::ofstream& operator<<(std::ifstream& is, l_table& t);

} //~namespace pbd

#endif // PBD_L_TABLE_H
