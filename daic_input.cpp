#include "daic_input.h"

#include <fstream>
#include <iostream>
#include "daic_input_row.h"

daic::input::input(const std::vector<input_row>& table) noexcept
  : m_table{table}
{

}

daic::input daic::create_input_article() noexcept
{
  return input(
    {
      input_row("Coccyzus"    , species_status::non_endemic_max_age, 0.0, { 7.456 } ),
      input_row("Dendroica"   , species_status::non_endemic        , 0.0, { 0.34  } ),
      input_row("Finches"     , species_status::endemic            , 0.0, { 3.0282,1.3227,0.8223,0.4286,0.3462,0.245,0.0808,0.0527,0.0327,0.0221,0.118,0.0756,0.0525,0.0322,0.0118 } ),
      input_row("Mimus"       , species_status::endemic            , 0.0, { 3.958,3.422,2.884,0.459 } ),
      input_row("Myiarchus"   , species_status::endemic            , 0.0, { 0.855 } ),
      input_row("Progne"      , species_status::endemic            , 0.0, { 0.086 } ),
      input_row("Pyrocephalus", species_status::non_endemic_max_age, 0.0, { 10.285 } ),
      input_row("Zenaida"     , species_status::endemic            , 0.0, { 3.51 } )
    }
  );
}

daic::input daic::create_input_article_light() noexcept
{
  return input(
    {
      input_row("Coccyzus" , species_status::non_endemic_max_age, 0.0, { 7.456 } ),
      input_row("Dendroica", species_status::non_endemic        , 0.0, { 0.34  } ),
      input_row("Finches"  , species_status::endemic            , 0.0, { 3.0282,1.3227,0.8223 } )
    }
  );
}

void daic::save(const input& i, const std::string& filename)
{
  std::ofstream f(filename);
  f << i;
}

std::ostream& daic::operator<<(std::ostream& os, const input& i) noexcept
{
  os << create_input_header() << '\n';
  for (const input_row& r: i.m_table)
  {
    os << r << '\n';
  }
  return os;
}

